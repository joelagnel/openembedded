def auto_assume_provided(d):
    from oe.assume import assume
    return assume(d)

ASSUME_PROVIDED += "${@auto_assume_provided(d)}"
