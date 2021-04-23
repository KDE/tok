import org.kde.Tok.Tests 1.0
import QtTest 1.15
import QtQuick 2.0

Rectangle {
    id: t
    color: "green"

    function screenshot(name) {
        // tcase.grabImage(rootWindow.contentItem).save(`/tmp/tok-test-screenshots/${name}.png`)
    }

    MainWindow {
        id: rootWindow

        property TestCase testCase: TestCase {
            id: tcase
            when: windowShown

            function test_aa_wake_up() {
                testEventFeeder.triggerStage("stageInitial")
                t.screenshot("screen-initial")
                wait(2*1000)
            }
            function test_ab_phone_number() {
                testEventFeeder.triggerStage("stageEntryNumber")
                t.screenshot("screen-entryNumber")
                wait(2*1000)
            }
            function test_ac_code() {
                testEventFeeder.triggerStage("stageEntryCode")
                t.screenshot("screen-entryCode")
                wait(2*1000)
            }
            function test_ad_password() {
                testEventFeeder.triggerStage("stageEntryPassword")
                t.screenshot("screen-entryPassword")
                wait(2*1000)
            }
            function test_ae_blank_chats() {
                testEventFeeder.triggerStage("stageNoChats")
                t.screenshot("screen-noChats")
                wait(2*1000)
            }
        }
    }
}
