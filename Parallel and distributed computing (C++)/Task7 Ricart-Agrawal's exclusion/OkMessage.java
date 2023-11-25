package cz.cvut.fel.agents.pdv.exclusion;

import cz.cvut.fel.agents.pdv.clocked.ClockedMessage;

public class OkMessage extends ClockedMessage {
    private final String csn;
    public OkMessage(String criticalSectionName)
    {
        csn = criticalSectionName;
    }
    public String getCriticalSectionName() {
        return csn;
    }
}