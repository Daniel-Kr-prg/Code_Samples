package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;
import cz.cvut.fel.agents.pdv.dsand.Pair;

import java.util.*;
import java.util.Map.Entry;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * Trida s implementaci metody act() pro proces Failure Detector. Tuto tridu (a tridy pouzivanych zprav) budete
 * odevzdavat. Do tridy si muzete doplnit vlastni pomocne datove struktury. Hodnoty muzete inicializovat primo
 * v konstruktoru. Klicova je metoda act(), kterou vola kazda instance tridy FailureDetectorProcess ve sve metode
 * act(). Tuto metodu naimplementujte podle protokolu SWIM predstaveneho na prednasce.
 *
 * Pokud si stale jeste nevite rady s frameworkem, inspiraci muzete nalezt v resenych prikladech ze cviceni.
 */
public class ActStrategy {

    // maximalni zpozdeni zprav
    private final int maxDelayForMessages;
    private final List<String> otherProcesses;

    private final int timeToDetectKilledProcess;
    private final int upperBoundOnMessages;

    // Definujte vsechny sve promenne a datove struktury, ktere budete potrebovat
    private final Random random = new Random();

    private final int pingWaitTime;
    private final int requestRespondWaitTime;
    private final int pingPeriodTime;

    private List<Pair<String, Integer>> pingSent;
    private List<Pair<PingRequest, Integer>> waitForRespondFrom;
    private List<Pair<PingRequest, Integer>> respondTo;

    private final int REQUEST_MESSAGES = 3;
    int tickCounter = 0;

    private String myIndex = "unknown";

    private Integer currId;
    private boolean allowDebug = false;
    public ActStrategy(int maxDelayForMessages, List<String> otherProcesses,
                       int timeToDetectKilledProcess, int upperBoundOnMessages) {
        this.maxDelayForMessages = maxDelayForMessages;
        this.otherProcesses = otherProcesses;
        this.timeToDetectKilledProcess = timeToDetectKilledProcess;
        this.upperBoundOnMessages = upperBoundOnMessages;

        pingWaitTime = 2 * maxDelayForMessages;
        requestRespondWaitTime = 4 * maxDelayForMessages;
        pingPeriodTime = (timeToDetectKilledProcess-maxDelayForMessages*6)/2;

        pingSent = new ArrayList<>();
        waitForRespondFrom = new ArrayList<>();
        respondTo = new ArrayList<>();
        currId = 0;
    }

    /**
     * Metoda je volana s kazdym zavolanim metody act v FailureDetectorProcess. Metodu implementujte
     * tak, jako byste implementovali metodu act() v FailureDetectorProcess, misto pouzivani send()
     * pridejte zpravy v podobe paru - prijemce, zprava do listu. Zpravy budou nasledne odeslany.
     * <p>
     * Diky zavedeni teto metody muzeme kontrolovat pocet odeslanych zprav vasi implementaci.
     */
    public List<Pair<String, Message>> act(Queue<Message> inbox, String disseminationProcess) {
        // Od DisseminationProcess muzete dostat zpravu typu DeadProcessMessage, ktera Vas
        // informuje o spravne detekovanem ukoncenem procesu.
        // DisseminationProcess muzete poslat zpravu o detekovanem "mrtvem" procesu.
        // Zprava musi byt typu PFDMessage.

        List<Pair<String, Message>> messagesToSend = new ArrayList<>();


        while (!inbox.isEmpty())
        {
            Message message = inbox.poll();
            if (myIndex.equals("unknown"))
                myIndex = message.recipient;
            if (message instanceof DeadProcessMessage)
            {
                HandleDeadProcessMessage(message);
            }
            else if (message instanceof PingRequest)
            {
                HandlePingRequest(message, messagesToSend);
            }
            else if (message instanceof PingMessage)
            {
                HandlePingMessage(message, messagesToSend);
            }
        }

        tickCounter++;

        String finalMyIndex = myIndex;
        if (tickCounter % pingPeriodTime == 0) {
            String randomProcess = otherProcesses.get(random.nextInt(otherProcesses.size()));
            messagesToSend.add(new Pair<>(randomProcess, new PingMessage(false, currId)));
            currId++;
            pingSent.add(new Pair<>(randomProcess, tickCounter + pingWaitTime));
            if (allowDebug)
                System.out.println(finalMyIndex + ": sending PING to: " + randomProcess);
        }


        pingSent.removeIf(pair -> {
            if (tickCounter > pair.getSecond()) {
                String target = pair.getFirst();
                if (allowDebug)
                    System.out.println(finalMyIndex + ": PING timeout: " + target);
                for (int i = 0; i < REQUEST_MESSAGES; i++) {
                    String kRandomProcess = otherProcesses.get(random.nextInt(otherProcesses.size()));
                    while (kRandomProcess.equals(target))
                        kRandomProcess = otherProcesses.get(random.nextInt(otherProcesses.size()));
                    PingRequest newRequest = new PingRequest(target, false, currId);
                    if (allowDebug)
                        System.out.println(finalMyIndex + ": sending REQUEST to: " + kRandomProcess + " about: " + target);
                    newRequest.recipient = kRandomProcess;
                    messagesToSend.add(new Pair<>(kRandomProcess, newRequest));
                    waitForRespondFrom.add(new Pair<>(newRequest, tickCounter + requestRespondWaitTime));
                }
                currId++;
                return true;
            }
            return false;
        });

        respondTo.removeIf(pair -> {
            if (tickCounter > pair.getSecond())
            {
                if (allowDebug)
                    System.out.println(finalMyIndex + ": PING timeout in respondTo: " + pair.getFirst().sender + " about: " + pair.getFirst().getTarget());
                return true;
            }
            return false;
        });

        List<String> removed = new ArrayList<>();
        waitForRespondFrom.removeIf(pair -> {
            if (tickCounter > pair.getSecond()) {
                String deadProcess = pair.getFirst().getTarget();
                if (!removed.contains(deadProcess)) {
                    messagesToSend.add(new Pair<>(disseminationProcess, new PFDMessage(deadProcess)));
                    removed.add(deadProcess);
                }
                if (allowDebug)
                    System.out.println(finalMyIndex + ": REQUEST timeout in waitForRespondFrom: " + pair.getFirst().recipient + " about: " + pair.getFirst().getTarget());
                return true;
            }
            return false;
        });

        return messagesToSend;
    }

    private void RemoveUnresolvedPing(String procID)
    {
        pingSent.removeIf(pair -> pair.getFirst().equals(procID));

        waitForRespondFrom.removeIf(pair -> pair.getFirst().getTarget().equals(procID));
        waitForRespondFrom.removeIf(pair -> pair.getFirst().recipient.equals(procID));

        respondTo.removeIf(pair -> pair.getFirst().getTarget().equals(procID));
        respondTo.removeIf(pair -> pair.getFirst().sender.equals(procID));

        if (allowDebug)
            System.out.println("Clearing lists of dead process");
    }

    private void HandlePingMessage(Message message, List<Pair<String, Message>> messagesToSend)
    {
        PingMessage pingMessage = (PingMessage) message;
        if (pingMessage.isAck())
        {
            pingSent.removeIf(pair -> pair.getFirst().equals(pingMessage.sender));
            //pingSent.removeIf(pair -> { return pair.getFirst().equals(pingMessage.sender) && pair.getFirst(). });
            if (allowDebug)
                System.out.println(pingMessage.recipient + ": received PING RESPONSE from: " + pingMessage.sender);
            for (Iterator<Pair<PingRequest, Integer>> iterator = respondTo.iterator(); iterator.hasNext();) {
                Pair<PingRequest, Integer> element = iterator.next();
                PingRequest request = element.getFirst();
                if (request.getTarget().equals(pingMessage.sender))
                {
                    messagesToSend.add(new Pair<>(request.sender, new PingRequest(request.getTarget(), true, request.getId())));
                    iterator.remove();
                    if (allowDebug)
                        System.out.println(request.recipient + ": sending REQUEST RESPONSE to: " + request.sender + " about: " + request.getTarget());
                }
            }
        }
        else
        {
            String sender = pingMessage.sender;
            messagesToSend.add(new Pair<>(sender, new PingMessage(true, pingMessage.getId())));

            //pingSent.add(new Pair<>(sender, tickCounter + pingWaitTime));
            if (allowDebug)
                System.out.println(pingMessage.recipient + ": received PING from: " + sender + ". Sending back");
        }
    }

    private void HandlePingRequest(Message message, List<Pair<String, Message>> messagesToSend)
    {
        PingRequest pingRequest = (PingRequest) message;
        if (pingRequest.isAck())
        {
            waitForRespondFrom.removeIf(pair -> pair.getFirst().getTarget().equals(pingRequest.getTarget()));
            //waitForRespondFrom.removeIf(pair -> { return pair.getFirst().getTarget().equals(pingRequest.getTarget()) && pingRequest.getId() == pair.getFirst().getId(); } );
            if (allowDebug) {
                System.out.println(pingRequest.recipient + ": received REQUEST RESPONSE from: " + pingRequest.sender + " about: " + pingRequest.getTarget());
            }
        }
        else
        {
            String target = pingRequest.getTarget();
            messagesToSend.add(new Pair<>(target, new PingMessage(false, currId)));
            currId++;
            respondTo.add(new Pair<>(pingRequest, tickCounter + requestRespondWaitTime));
            if (allowDebug)
                System.out.println(pingRequest.recipient + ": received REQUEST from: " + pingRequest.sender + " about: " + target);
        }
    }

    private void HandleDeadProcessMessage(Message message)
    {
        DeadProcessMessage deadMessage = (DeadProcessMessage) message;
        otherProcesses.remove(deadMessage.getProcessID());
        RemoveUnresolvedPing(deadMessage.getProcessID());
        if (allowDebug)
            System.out.println("DEAD: " + deadMessage.getProcessID());
    }

}
