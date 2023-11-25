package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class RequestMessage extends ClockedMessage {

    private final String csn;
    public RequestMessage(String criticalSectionName)
    {
        csn = criticalSectionName;
    }

    public String getCriticalSectionName() {
        return csn;
    }
}
