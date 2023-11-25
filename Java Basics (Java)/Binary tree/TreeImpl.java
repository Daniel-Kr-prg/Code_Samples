package cz.cvut.fel.pjv;

public class TreeImpl implements Tree {

    private NodeImpl root;

    public TreeImpl()
    {
        root = null;
    }

    public void setTree(int[] values)
    {
        if (values.length > 0)
            root = new NodeImpl(values);
        else
            root = null;
    }
    /**
     * @return root of the tree, or null if the tree is empty.
     */
    public Node getRoot()
    {
        return root;
    }

    /**
     * Tree description looks as follows (for tree created from values [1, 2, 3, 4, 5, 6, 7]):
     * - 4
     *  - 2
     *   - 1
     *   - 3
     *  - 6
     *   - 5
     *   - 7
     *
     * @return String representation of the tree, or "" for empty tree
     */
    public String toString()
    {
        String res = "";
        if (root != null)
        {
            res = getSubString(getRoot(), 0);
        }
        return res;
    }
    private String getSubString(Node current, int spaces)
    {
        String space = " ";
        String res = "";
        for (int i = 0; i < spaces; i++)
            res += space;
        res += "- " +current.getValue() + "\n";
        if (current.getLeft() != null)
        {
            res += getSubString(current.getLeft(), spaces+1);
            if (current.getRight() != null)
            {
                res += getSubString(current.getRight(), spaces+1);
            }
        }
        return res;
    }
}
