package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;
import cz.cvut.fel.agents.pdv.clocked.ClockedProcess;

import java.util.HashSet;
import java.util.Set;

public class ExclusionPrimitive {

    /**
     * Stavy, ve kterych se zamek muze nachazet.
     */
    enum AcquisitionState {
        RELEASED,    // Uvolneny   - Proces, ktery vlastni aktualni instanci ExclusionPrimitive o pristup do kriticke
                     //              sekce nezada

        WANTED,      // Chteny     - Proces, ktery vlastni aktualni instanci ExclusionPrimitive zada o pristup do
                     //              kriticke sekce. Ten mu ale zatim nebyl odsouhlasen ostatnimi procesy.

        HELD         // Vlastneny  - Procesu bylo prideleno pravo pristupovat ke sdilenemu zdroji.
    }

    private ClockedProcess owner;            // Proces, ktery vlastni aktualni instanci ExclusionPrimitive

    private String criticalSectionName;      // Nazev kriticke sekce. POZOR! V aplikaci se muze nachazet vice kritickych
                                             // sekci s ruznymi nazvy!

    private String[] allAccessingProcesses;  // Seznam vsech procesu, ktere mohou chtit pristupovat ke kriticke sekci
                                             // s nazvem 'criticalSectionName' (a tak spolurozhoduji o udelovani pristupu)

    private AcquisitionState state;          // Aktualni stav zamku (vzhledem k procesu 'owner').
                                             // state==HELD znamena, ze proces 'owner' muze vstoupit do kriticke sekce

    // Doplnte pripadne vlastni datove struktury potrebne pro implementaci
    // Ricart-Agrawalova algoritmu pro vzajemne vylouceni

    private int repliesReceived;
    private int requestTime;
    private Set<String> deferredReplies;

    public ExclusionPrimitive(ClockedProcess owner, String criticalSectionName, String[] allProcesses) {
        this.owner = owner;
        this.criticalSectionName = criticalSectionName;
        this.allAccessingProcesses = allProcesses;
        this.state = AcquisitionState.RELEASED;
        this.repliesReceived = 0;
        this.deferredReplies = new HashSet<>();
    }

    /**
     * Metoda pro zpracovani nove prichozi zpravy
     *
     * @param m    prichozi zprava
     * @return 'true', pokud je zprava 'm' relevantni pro aktualni kritickou sekci.
     */
    public boolean accept(ClockedMessage m) {

        // Implementujte zpracovani prijimane zpravy informujici
        // o pristupech ke sdilenemu zdroji podle Ricart-Agrawalova
        // algoritmu. Pokud potrebujete posilat specificke zpravy,
        // vytvorte si pro ne vlastni tridy.
        //
        // POZOR! Ne vsechny zpravy, ktere v teto metode dostanete Vas
        // budou zajimat! Budou Vam prichazet i zpravy, ktere se  napriklad
        // tykaji jinych kritickych sekci. Pokud je zprava nerelevantni, tak
        // ji nezpracovavejte a vratte navratovou hodnotu 'false'. Nekdo jiny
        // se o ni urcite postara :-)
        //
        // Nezapomente se starat o cas procesu 'owner'
        // pomoci metody owner.increaseTime(). Aktualizaci
        // logickeho casu procesu s prijatou zpravou (pomoci maxima) jsme
        // za Vas jiz vyresili.
        //
        // Cas poslani prijate zpravy muzete zjistit dotazem na hodnotu
        // m.sentOn. Aktualni logicky cas muzete zjistit metodou owner.getTime().
        // Zpravy se posilaji pomoci owner.send(prijemce, zprava) a je jim auto-
        // maticky pridelen logicky cas odeslani. Retezec identifikujici proces
        // 'owner' je ulozeny v owner.id.

        if (m instanceof RequestMessage) {
            RequestMessage request = (RequestMessage) m;
            if (!request.getCriticalSectionName().equals(criticalSectionName)) {
                return false;
            }
            owner.increaseTime();


            if (state == AcquisitionState.RELEASED ||
                    (state == AcquisitionState.WANTED &&
                            (requestTime > request.sentOn ||
                                    (requestTime == request.sentOn && owner.id.compareTo(request.sender) > 0)))) {
                owner.send(request.sender, new OkMessage(criticalSectionName));
            } else {
                deferredReplies.add(request.sender);
            }

            return true;
        }
        else if (m instanceof OkMessage)
        {
            OkMessage ok = (OkMessage) m;
            if (!ok.getCriticalSectionName().equals(criticalSectionName)) {
                return false;
            }
            repliesReceived++;

            if (repliesReceived == allAccessingProcesses.length - 1 && state == AcquisitionState.WANTED)
            {
                state = AcquisitionState.HELD;
            }

            return true;
        }

        return false;
    }

    public void requestEnter() {

        owner.increaseTime();

        state = AcquisitionState.WANTED;
        repliesReceived = 0;
        requestTime = owner.getTime();
        deferredReplies.clear();

        for (String process : allAccessingProcesses) {
            if (!process.equals(owner.id)) {
                owner.send(process, new RequestMessage(criticalSectionName));
            }
        }

    }

    public void exit() {

        // Implementuje uvolneni zdroje, aby k nemu meli pristup i
        // ostatni procesy z 'allAccessingProcesses', ktere ke zdroji
        // mohou chtit pristupovat


        state = AcquisitionState.RELEASED;

        owner.increaseTime();
        for (String process : deferredReplies) {
            owner.send(process, new OkMessage(criticalSectionName));
        }
        deferredReplies.clear();

    }

    public String getName() {
        return criticalSectionName;
    }

    public boolean isHeld() {
        return state == AcquisitionState.HELD;
    }

}
