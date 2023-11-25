package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class PingRequest extends PingMessage {
    private final String target;
    public PingRequest(String target, boolean ack, Integer id) {
        super(ack, id);
        this.target = target;
    }

    public String getTarget() {
        return target;
    }

}
