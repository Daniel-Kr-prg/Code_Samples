package cz.cvut.fel.pjv;

public class BruteForceAttacker extends Thief {
    
    @Override
    public void breakPassword(int sizeOfPassword) {
        char[] str = new char[sizeOfPassword];
        for (int i = 0; i < sizeOfPassword; i++) {
            str[i] = getCharacters()[0];
        }
        if (sizeOfPassword > 0)
            buildString(sizeOfPassword, 1, str);
        else
            tryOpen(new char[0]);
    }

    private void buildString(int passwordSize, int currentSize, char[] currentString)
    {
        for (int i = 0; (!this.isOpened()) && (i < getCharacters().length); i++)
        {
            currentString[currentSize-1] = getCharacters()[i];

            if (currentSize == passwordSize)
            {
                tryOpen(currentString);
            }
            else
            {
                buildString(passwordSize, currentSize+1, currentString);
            }
        }
    }

    
}
