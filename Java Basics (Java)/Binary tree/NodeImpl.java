package cz.cvut.fel.pjv;

import java.util.Arrays;

public class NodeImpl implements Node{
    public NodeImpl left = null;
    public NodeImpl right = null;
    public int value;

    public NodeImpl(int[] values)
    {
        int center = values.length / 2;
        value = values[center];
        if (values.length > 1)
            left = new NodeImpl(Arrays.copyOfRange(values, 0, center));
        if (values.length > 2)
            right = new NodeImpl(Arrays.copyOfRange(values, center+1, values.length));
    }

    public Node getLeft()
    {
        return left;
    }


    public Node getRight()
    {
        return right;
    }

    public int getValue()
    {
        return value;
    }
}
