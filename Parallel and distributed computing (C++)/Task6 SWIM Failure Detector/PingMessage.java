package cz.cvut.fel.agents.pdv.swim;

import cz.cvut.fel.agents.pdv.dsand.Message;

public class PingMessage extends Message {

    private final boolean ack;
    private final Integer id;
    public PingMessage(boolean ack, Integer id) {
        this.ack = ack;
        this.id = id;
    }
    public boolean isAck()
    {
        return ack;
    }
    public Integer getId() { return id; }
}
