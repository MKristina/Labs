package Master;

import Message.MessageType;
import Main.Packet;
import Main.TreeNode;

import java.net.InetSocketAddress;
import java.util.concurrent.ConcurrentLinkedQueue;

public class RecvMaster implements Runnable {
    TreeNode node;
    public RecvMaster(TreeNode node) {
        this.node = node;
    }

    @Override
    public void run() {
        ConcurrentLinkedQueue<Packet> recvMessage = node.getRecvPackets();
        ConcurrentLinkedQueue<Packet> SavedPacketsToSend = node.getSavedPacketsToSend();
        ConcurrentLinkedQueue<Packet> packetsToSend = node.getPacketsToSend();
        while (true) {
            Packet packet = recvMessage.poll();
            if (packet != null) {
                if (packet.getMessage().getMessageType() != MessageType.PING_MESSAGE) {
                    System.out.println(packet.getMessage().getName()+": " + packet.getMessage().getMessageText() + "////" + packet.getInetSocketAddress().toString());
                }
                switch (packet.getMessage().getMessageType()) {
                    case ACCEPT_CHILD_MESSAGE : {
                        node.setParent(packet.getInetSocketAddress());
                        SavedPacketsToSend.removeIf(item -> item.getMessage().getGUID().equals(packet.getMessage().getGUID()));
                        System.out.println("I'm got parent now! it's :" + node.getParent().toString());
                    }
                    case ADOPT_CHILD_MESSAGE : {
                        node.addChild(packet.getInetSocketAddress());
                        Packet.answerToPacket(packet, MessageType.ACCEPT_CHILD_MESSAGE, "I  accept you", packetsToSend, Packet.ACCEPT_CHILD_TTL,node.getName());
                        if (!node.hasAlterNode()) {
                            node.setAlterNode(packet.getInetSocketAddress());
                            int port;
                            if(node.hasParent()) {
                                port = node.getParent().getPort();
                            }
                            else port = node.getSocket().getLocalPort();
                            var magicFoster = new InetSocketAddress("127.0.0.1",port);

                            Packet.answerToPacket(packet,MessageType.SEND_FOSTER_MESSAGE,magicFoster.toString() ,packetsToSend,Packet.SEND_FOSTER_TTL,node.getName());
                            Packet.spreadPacket(packet, packetsToSend, Packet.SEND_FOSTER_TTL,node,true);
                            System.out.println("i got alter node" + packet.getInetSocketAddress().toString());
                        } else {
                            Packet.answerToPacket(packet,MessageType.SEND_FOSTER_MESSAGE,node.getAlterNode().toString(),packetsToSend,Packet.SEND_FOSTER_TTL, node.getName());
                        }
                        System.out.println("I send alter node to my children" + node.getAlterNode());
                    }
                    case CHAT_MESSAGE : {
                        Packet.answerToPacket(packet, MessageType.ACCEPT_CHAT_MESSAGE, "I"+" accept message from you", packetsToSend, Packet.ACCEPT_CHAT_MESSAGE_TTL, node.getName());
                        Packet.spreadPacket(packet, packetsToSend, Packet.CHAT_MESSAGE_TTL,node, false);
                    }
                    case ACCEPT_CHAT_MESSAGE : {
                        SavedPacketsToSend.removeIf(item -> item.getMessage().getGUID().equals(packet.getMessage().getGUID()));
                        System.out.println(packet.getInetSocketAddress().toString() + "//" + packet.getMessage().getName() + " got my message");
                    }
                    case PING_MESSAGE : {
                        node.updateRelatives(packet.getInetSocketAddress());
                    }
                    case SEND_FOSTER_MESSAGE : {
                        Packet.answerToPacket(packet,MessageType.CONFIRM_FOSTER_MESSAGE,"got foster",packetsToSend,Packet.CONFIRM_FOSTER_TTL,node.getName());
                        String[] data = packet.getMessage().getMessageText().split(":");
                        data[0] = data[0].substring(1);
                        InetSocketAddress fosterAddr = new InetSocketAddress(data[0], Integer.parseInt(data[1]));
                        node.setFosterParent(fosterAddr);
                        System.out.println("i got a foster: " + fosterAddr.toString());

                    }
                    case CONFIRM_FOSTER_MESSAGE : {
                        SavedPacketsToSend.removeIf(item -> item.getMessage().getGUID().equals(packet.getMessage().getGUID()));
                    }
                }


            }
        }


    }




}
