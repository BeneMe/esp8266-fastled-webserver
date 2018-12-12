Import("env", "projenv")

# access to global build environment
print env

# access to project build environment (is used source files in "src" folder)
print projenv

def before_upload(source, target, env):
    print "before_upload"
    

def before_build_spiffs(source, target, env):
    print "Cleaning webapp"
    projenv.Execute("npm --prefix ./webapp/ run clean")
    print "Building webapp"
    projenv.Execute("npm --prefix ./webapp/ run build")
    # projenv.Execute("gzip -r ./data")
 
env.AddPreAction(".pioenvs/%s/spiffs.bin" % env['PIOENV'], before_build_spiffs)

env.AddPreAction("upload", before_upload)