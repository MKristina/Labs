package Master;

import Main.TreeNode;
import java.util.TimerTask;

public class MessageBufferMaster extends TimerTask {
    private TreeNode node;
    public MessageBufferMaster(TreeNode node){
        this.node = node;
    }

    @Override
    public void run() {
        var savedPacketsToSend = node.getSavedPacketsToSend();
        savedPacketsToSend.removeIf(item ->item.getTtl() < 1);
        System.err.println("removed");
        System.err.println(savedPacketsToSend);
        var packetsToSend = node.getPacketsToSend();
        packetsToSend.addAll(savedPacketsToSend);


    }
}
