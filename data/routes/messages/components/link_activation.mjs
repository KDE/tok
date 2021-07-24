/**
 * @param {string} str - The username or user ID
 */
function handleMention(str, globalUserDataSheet, client) {
    if (!isNaN(str)) { // is numeric ID
        globalUserDataSheet.userID = str
        globalUserDataSheet.open()
    } else { // is username
        client.searchPublicChat(str).then((data) => {
            if (data.type === "private") {
                globalUserDataSheet.userID = data.chatID
                globalUserDataSheet.open()
            }
        })
    }
}
/**
 * Handles a link as appropriate.
 *
 * @param {string} str - The link to handle
 */
export function handle(str, globalUserDataSheet, client) {
    if (str[0] === "@") {
        handleMention(str.substring(1), globalUserDataSheet, client)
    } else {
        Qt.openUrlExternally(str)
    }
}