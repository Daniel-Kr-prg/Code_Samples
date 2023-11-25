package cz.cvut.fel.pjv;

/**
 * Implementation of the {@link Queue} backed by fixed size array.
 */
public class CircularArrayQueue implements Queue {

    int Capacity;
    int Head = 0;
    int Tail = 0;

    String[] QArr;
    /**
     * Creates the queue with capacity set to the value of 5.
     */
    public CircularArrayQueue() {
        Capacity = 5;
        QArr = new String[Capacity];
    }


    /**
     * Creates the queue with given {@code capacity}. The capacity represents maximal number of elements that the
     * queue is able to store.
     * @param capacity of the queue
     */
    public CircularArrayQueue(int capacity) {
        Capacity = capacity;
        QArr = new String[Capacity];
    }

    @Override
    public int size() {
        if (isEmpty())
            return 0;
        return Tail > Head ? Tail - Head : Tail + Capacity - Head;
    }

    @Override
    public boolean isEmpty()
    {
        if ((Head == Tail) && (QArr[Tail] == null))
            return true;
        return false;
    }

    @Override
    public boolean isFull() {
        if ((Head == Tail) && (QArr[Tail] != null))
            return true;
        return false;
    }

    @Override
    public boolean enqueue(String obj) {
        if (isFull())
            return false;
        QArr[Tail] = obj;
        Tail = Tail + 1 == Capacity ? 0 : Tail + 1;
        return true;
    }

    @Override
    public String dequeue() {
        if (isEmpty())
            return null;
        String ret = QArr[Head];
        QArr[Head] = null;
        Head = Head + 1 == Capacity ? 0 : Head + 1;
        return ret;
    }

    @Override
    public void printAllElements() {
        for (int i = 0; i < size(); i++)
        {
            System.out.println(QArr[(Head + i) % Capacity]);
        }
    }
}
