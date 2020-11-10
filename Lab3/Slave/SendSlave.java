package Slave;

import Message.Message;
import Message.MessageType;
import Main.Packet;
import Main.TreeNode;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.util.UUID;
import java.util.concurrent.ConcurrentLinkedQueue;

public class SendSlave implements Runnable {
    DatagramSocket socket;
    TreeNode node;


    public SendSlave(TreeNode node) {
        this.socket = node.getSocket();
        this.node = node;
    }

    @Override
    public void run() {
        ConcurrentLinkedQueue<Packet> packetsToSend = node.getPacketsToSend();
        ConcurrentLinkedQueue<Packet> SavedPacketsToSend = node.getSavedPacketsToSend();
        if (node.hasParent()) {
            Message adoptMessage = new Message(MessageType.ADOPT_CHILD_MESSAGE, node.getName(), "127.0.0.1:"+socket.getLocalPort(), UUID.randomUUID());
            Packet adoptPacket = new Packet(node.getParent(), adoptMessage, Packet.ADOPT_CHILD_TTL);
            packetsToSend.add(adoptPacket);
        }
        while (true) {
            try {
                Packet packet = packetsToSend.poll();
                if (packet != null) {
                    if (packet.getMessage().getMessageType() != MessageType.PING_MESSAGE) {
                        packet.decTtl();
                        SavedPacketsToSend.add(packet);
                    }
                    if (packet.getMessage().getMessageType() != MessageType.PING_MESSAGE) {
                       System.out.println("type: " + packet.getMessage().getMessageType());
                       System.out.println("ttl: " + packet.getTtl());
                       System.out.println("send: " + packet.getMessage().getMessageText() + " to " + packet.getInetSocketAddress().toString());
                   }
                    sendMessage(packet.getMessage(), packet.getInetSocketAddress());
                }

            } catch (IOException e) {
                e.printStackTrace();
            }

        }

    }

    void sendMessage(Message message, SocketAddress addPort) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream(Message.BUFF_SIZE);
        ObjectOutputStream oos = new ObjectOutputStream(baos);
        oos.writeObject(message);
        DatagramPacket packet = new DatagramPacket(baos.toByteArray(), baos.toByteArray().length, addPort);
        socket.send(packet);

    }
}


