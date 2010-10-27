python deprecated_rm_work() {
    if isinstance(e, bb.event.BuildStarted):
        bb.msg.warn(None, "The rm_work class is now unnecessary and deprecated")
}
addhandler deprecated_rm_work
