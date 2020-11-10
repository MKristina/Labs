package Message;

import java.io.Serializable;
import java.util.UUID;

public class Message implements Serializable {
    public final static int BUFF_SIZE = 2048;

    private final MessageType messageType;
    private final String  messageText;
    private final String name;
    private final UUID GUID;

    public Message(MessageType messageType,String hostname, String messageText, UUID guid) {
        this.messageType = messageType;
        this.messageText = messageText;
        this.name = hostname;
        this.GUID = guid;
    }

    public String getName() {
        return name;
    }

    public MessageType getMessageType() {
        return messageType;
    }

    public String getMessageText() {
        return messageText;
    }

    public UUID getGUID() {
        return GUID;
    }
}
