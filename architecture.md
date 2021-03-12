# architecture

## QML

QML files go in `data`.

Plan looks something like this:
- `main.qml`
- `routes/*.qml` for various "screens"
- `components/*.qml` for shared components
- `routes/*/*.qml` for route-specific component

## C++ classes:

- `Client::Private`: Should do all if not most interaction with TDLib.
- `Client`: Exposes the "entrypoint" to the UI layer
    - Currently handles login sequence.
    - Will be provide ChatModels that handle a message stream.
    - Everything under a Client should be parented to it and should cleanly handle
      the Client destructing at any time.
    - May possibly have multiple Clients for multiple accounts. Need to investigate
      how to handle with TDLib.
- `ChatModel`: The model of messages for a chat.
    - Should we have subclasses for different types of chats? There's a few distinct types:
        - Bot
        - Private Group
        - Public Group
        - Supergroup
        - Channel
        - System
        - Secret Chat

## Other Things

Rendering animated stickers will need an implementation of Telegram's Lottie subset. Wonder if
QtLottie can do it all.
