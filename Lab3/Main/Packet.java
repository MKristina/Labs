package Main;

import Message.Message;
import Message.MessageType;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetSocketAddress;
import java.util.UUID;
import java.util.concurrent.ConcurrentLinkedQueue;

public class Packet {
    public final static int ACCEPT_CHILD_TTL = 1;
    public final static int ACCEPT_CHAT_MESSAGE_TTL = 1;
    public final static int ADOPT_CHILD_TTL = 2;
    public final static int CHAT_MESSAGE_TTL = 4;
    public final static int PING_TTL = 1;
    public final static int CONFIRM_FOSTER_TTL = 1;
    public final static int SEND_FOSTER_TTL = 2;
    private int ttl;
    private InetSocketAddress socketAddress;
    private Message message;

    public int getTtl() {
        return ttl;
    }

    public Packet(InetSocketAddress socketAddress, Message message, int ttl) {
        this.socketAddress = socketAddress;
        this.message = message;
        this.ttl = ttl;
    }

    public void decTtl() {
        ttl -= 1;
    }

    public InetSocketAddress getInetSocketAddress() {
        return socketAddress;
    }

    public Message getMessage() {
        return message;
    }

    public void setInetSocketAddress(InetSocketAddress socketAddress) {
        this.socketAddress = socketAddress;
    }

    public void setMessage(Message message) {
        this.message = message;
    }


    static public Packet getPacket(DatagramSocket socket, int recTTl) throws IOException, ClassNotFoundException {
        DatagramPacket recvPacket = new DatagramPacket(new byte[Message.BUFF_SIZE], Message.BUFF_SIZE);
        socket.receive(recvPacket);
        ObjectInputStream ois = new ObjectInputStream(new ByteArrayInputStream(recvPacket.getData()));
        Message recvMessage = (Message) ois.readObject();

        return new Packet(new InetSocketAddress(recvPacket.getAddress(),recvPacket.getPort()) , recvMessage,recTTl);

    }

    static public void answerToPacket(Packet packet, MessageType type, String text, ConcurrentLinkedQueue<Packet> packetsToSend, int ttl, String dstName) {
        Message message = new Message(type, dstName, text, packet.getMessage().getGUID());
        Packet answerPacket = new Packet(packet.getInetSocketAddress(), message, ttl);
        packetsToSend.add(answerPacket);
    }

    static public void spreadPacket(Packet packet, ConcurrentLinkedQueue<Packet> packetsToSend, int ttl, TreeNode node, boolean onlyChildren) {
        InetSocketAddress src = packet.getInetSocketAddress();
        ConcurrentLinkedQueue<InetSocketAddress> children = node.getChildren();
        Packet packetToSpread;
        Message messageToSpread;
        if (node.hasParent() && !onlyChildren) {
            if (!src.equals(node.getParent())) {
                messageToSpread = new Message(packet.getMessage().getMessageType(),packet.getMessage().getName(),packet.getMessage().getMessageText(), UUID.randomUUID());
                packetToSpread = new Packet(node.getParent(),messageToSpread, ttl);
                packetsToSend.add(packetToSpread);
            }

        }
        for (InetSocketAddress sendAddr : children) {
            if (!src.equals(sendAddr)) {
                messageToSpread =  new Message(packet.getMessage().getMessageType(),packet.getMessage().getName(),packet.getMessage().getMessageText(), UUID.randomUUID());
                packetToSpread = new Packet(sendAddr, messageToSpread, ttl);
                packetsToSend.add(packetToSpread);
            }

        }


    }
}


