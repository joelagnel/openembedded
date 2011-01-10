import oe.process

assumptions = {}


def can_run(cmd):
    try:
        oe.process.Popen(cmd, shell=False)
    except (IOError, OSError):
        return False
    else:
        return True

def simple_assumptions(*cmds, **kwcmds):
    for cmd in cmds:
        assumptions['%s-native' % cmd] = lambda: can_run(cmd)

    for cmd, provide in kwcmds.iteritems():
        assumptions[provide] = lambda: can_run(cmd)

def assumed(assumption):
    def add_assumption(func):
        assumptions[assumption] = func
        return func
    return add_assumption


simple_assumptions(
    'fakeroot',
    'sed',
    'grep',
    'unzip',
    'svn',
    'patch',
    'diffstat',
    'cvs',
    'bzip2',
    'bc',
    'chrpath',
    'quilt',

    makeinfo='texinfo-native',
)


def assume(d):
    ret = []
    for assumption, testfunc in assumptions.iteritems():
        if testfunc():
            ret.append(assumption)
    return " ".join(ret)
