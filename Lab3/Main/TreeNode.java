package Main;

import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.net.SocketException;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class TreeNode {
    private final String name;
    private final int lossPercentage;
    private final DatagramSocket socket;
    private InetSocketAddress parent;
    private ConcurrentLinkedQueue<InetSocketAddress> children = new ConcurrentLinkedQueue<>();
    private ConcurrentLinkedQueue<Packet> recvPackets = new ConcurrentLinkedQueue<>();
    private ConcurrentLinkedQueue<Packet> packetsToSend = new ConcurrentLinkedQueue<>();
    private ConcurrentLinkedQueue<Packet> savedPacketsToSend = new ConcurrentLinkedQueue<>();
    private ConcurrentHashMap<InetSocketAddress, Long> relatives = new ConcurrentHashMap<>();
    private InetSocketAddress fosterParent;
    private InetSocketAddress alterNode;

    public InetSocketAddress getFosterParent() {
        return fosterParent;
    }

    public void setFosterParent(InetSocketAddress fosterParent) {
        this.fosterParent = fosterParent;
    }

    public InetSocketAddress getAlterNode() {
        return alterNode;
    }

    public void setAlterNode(InetSocketAddress alterNode) {
        this.alterNode = alterNode;
    }

    public TreeNode(String name, int port, int lossPercentage, InetSocketAddress parent) throws SocketException {
        this.name = name;
        this.socket = new DatagramSocket(port);
        this.lossPercentage = lossPercentage;
        this.parent = parent;
        this.fosterParent = null;
        this.alterNode = null;
    }

    public TreeNode(String name, int port, int lossPercentage) throws SocketException {
        this.name = name;
        this.socket = new DatagramSocket(port);
        this.lossPercentage = lossPercentage;
        this.parent = null;
        this.fosterParent = null;
        this.alterNode = null;
    }

    public void addChild(InetSocketAddress child) {
        children.add(child);
        relatives.put(child, System.nanoTime());
    }

    public void updateRelatives(InetSocketAddress aliveNode) {
        for (InetSocketAddress addr : relatives.keySet()) {
            if (addr.equals(aliveNode)) {
                relatives.replace(addr, System.nanoTime());
            }
        }
    }

    public Boolean forgetRelative(InetSocketAddress deadNode) {
        boolean parentIsDead = false;
        for (InetSocketAddress child : children) {
            if (child.equals(deadNode)) {
                children.remove(deadNode);
                break;
            }
        }
        if (hasParent()) {
            if (parent.equals(deadNode)) {
                parent = null;
                parentIsDead = true;
            }
        }
        relatives.entrySet().removeIf(item -> item.getKey().equals(deadNode));
        return parentIsDead;
    }


    public ConcurrentLinkedQueue<Packet> getRecvPackets() {
        return recvPackets;
    }
    public ConcurrentLinkedQueue<Packet> getPacketsToSend() {
        return packetsToSend;
    }

    public void setParent(InetSocketAddress parent) {
        this.parent = parent;
        if (parent != null) {
            relatives.put(parent, System.nanoTime());
        }
    }

    public void setChildren(ConcurrentLinkedQueue<InetSocketAddress> children) {
        this.children = children;
    }

    public String getName() {
        return name;
    }

    public int getLossPercentage() {
        return lossPercentage;
    }

    public DatagramSocket getSocket() {
        return socket;
    }

    public InetSocketAddress getParent() {
        return parent;
    }

    public ConcurrentLinkedQueue<InetSocketAddress> getChildren() {
        return children;
    }

    public ConcurrentLinkedQueue<Packet> getSavedPacketsToSend() {
        return savedPacketsToSend;
    }

    public ConcurrentHashMap<InetSocketAddress, Long> getRelatives() {
        return relatives;
    }

    public boolean hasParent() {
        return parent != null;
    }

    public boolean hasFosterParent() {
        return fosterParent != null;
    }

    public boolean hasAlterNode() {
        return alterNode != null;
    }

}
