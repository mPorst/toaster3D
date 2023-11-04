##########################################################
### Toaster prep-and-build script ########################
### checks out required external libraries when ##########
### required and triggers a cmake build for all ##########
### external libraries and internal toaster projects #####
##########################################################

from git import Repo
from argparse import ArgumentParser
import os
import shutil
import errno
import stat

### function definitions ###

def clear_dir(path):
    shutil.rmtree(path, ignore_errors=False, onerror=handle_remove_readonly)

def handle_remove_readonly(func, path, exc):
  excvalue = exc[1]
  if func in (os.rmdir, os.remove, os.unlink) and excvalue.errno == errno.EACCES:
    os.chmod(path, stat.S_IRWXU| stat.S_IRWXG| stat.S_IRWXO) # 0777
    func(path)
  else:
    raise
  
def checkout_repo(repo_url, repo_relpath, repo_tag, verbose_name):
  repo_fullpath = os.path.join(os.getcwd(), repo_relpath)
  if(not os.path.exists(repo_fullpath)):
    print("cloning " + verbose_name + "...")
    try:
      cloned_repo = Repo.clone_from(repo_url, repo_fullpath)
    except Exception as e:
      print("ERROR: problem occurred when cloning " + repo_url )
      print(e)
      print("exiting...")
      exit(1)
    try:
      cloned_repo.git.checkout(repo_tag)
    except Exception as e:
      print("WARNING: Could not checkout freetype tag " + repo_tag )
      print(e)
  else:
    print(verbose_name + " already exists, skipping cloning...")

print("## Start of toaster prep-and-build script ##")

### variables ###
# repo versions #
freetype_tag = "VER-2-13-2"
glfw_tag = "3.3.8"
# other repo data #
freetype_url = "https://github.com/freetype/freetype"
freetype_relpath = 'external/freetype'
glfw_url = "https://github.com/glfw/glfw"
glfw_relpath = 'external/glfw'

### Argument parsing ###

parser = ArgumentParser()
parser.add_argument("-fc", "--forceclean", dest="forceRebuild", action='store_true', help='force a rebuild')

cli_args = parser.parse_args()
if(cli_args.forceRebuild):
  print("force rebuild... deleting externally downloaded git repos")
  if(os.path.exists(freetype_relpath)):
    try:
      clear_dir(freetype_relpath)
    except Exception as e:
      print("ERROR: Could not delete " + freetype_relpath)
      print(e)
      print("exiting...")
      exit(1)
  if(os.path.exists(glfw_relpath)):
    try:
      clear_dir(glfw_relpath)
    except Exception as e:
      print("ERROR: Could not delete " + glfw_relpath)
      print(e)
      print("exiting...")
      exit(1)

### check out repositories ###
checkout_repo(freetype_url, freetype_relpath, freetype_tag, "Freetype")
checkout_repo(glfw_url, glfw_relpath, glfw_tag, "glfw")


print("## Reached end of toaster prep-and-build script ##")
