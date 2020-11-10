package Master;

import Message.*;
import Main.Packet;
import Main.TreeNode;

import java.net.InetSocketAddress;
import java.util.TimerTask;
import java.util.UUID;

public class PingMaster extends TimerTask {
    TreeNode node;
    final long DEAD_TIME = 10_000_000_000L;

    public PingMaster(TreeNode node) {
        this.node = node;
    }

    @Override
    public void run() {
        pingNodes();
        checkPing();
        checkAlterNode();
    }

    void pingNodes() {
        var children = node.getChildren();
        var packetsToSend = node.getPacketsToSend();
        Packet pingPacket;
        Message message;
        for (InetSocketAddress child : children) {
            message = new Message(MessageType.PING_MESSAGE, node.getName(), "alive", UUID.randomUUID());
            pingPacket = new Packet(child, message, Packet.PING_TTL);
            packetsToSend.add(pingPacket);
        }
        if (node.hasParent()) {
            message = new Message(MessageType.PING_MESSAGE, node.getName(), "alive", UUID.randomUUID());
            pingPacket = new Packet(node.getParent(), message, Packet.PING_TTL);
            packetsToSend.add(pingPacket);

        }

    }

    void checkPing() {
        boolean parentIsDead = false;
        for (InetSocketAddress item : node.getRelatives().keySet()) {
            long delta = System.nanoTime() - node.getRelatives().get(item);
            if (delta > DEAD_TIME) {
                parentIsDead = node.forgetRelative(item);
                System.out.println(item.toString() + " is dead");


            }
        }
        if (parentIsDead) {
            System.out.println("parent is dead");
            swapParent();

        }
    }


    void checkAlterNode() {
        if (!node.getChildren().contains(node.getAlterNode())) {
            swapAlterNode();

        }
    }

    private void swapParent() {
        node.setParent(node.getFosterParent());
        node.setFosterParent(null);
        if (node.hasParent()) {
            InetSocketAddress nodeAddr = new InetSocketAddress(node.getSocket().getInetAddress(), node.getSocket().getLocalPort());
            Message adoptMessage = new Message(MessageType.ADOPT_CHILD_MESSAGE, node.getName(), nodeAddr.toString(), UUID.randomUUID());
            Packet adoptPacket = new Packet(node.getParent(), adoptMessage, Packet.ADOPT_CHILD_TTL);
            node.getPacketsToSend().add(adoptPacket);
        }

    }

    private void swapAlterNode() {
        node.setAlterNode(node.getChildren().peek());
        if (node.getAlterNode() != null) {
            Message message = new Message(MessageType.SEND_FOSTER_MESSAGE, node.getName(), node.getAlterNode().toString(), UUID.randomUUID());
            Packet packet = new Packet(node.getAlterNode(), message, Packet.SEND_FOSTER_TTL);
            Packet.spreadPacket(packet, node.getPacketsToSend(), Packet.SEND_FOSTER_TTL, node, true);
        }

    }


}





