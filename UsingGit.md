# How to Use Git

First things first, make sure you have git on the machine you're running cFS
    
    sudo apt-get git

You can set local variables so your machine knows who you are
    
    git config --global user.name <YOUR USERNAME HERE>
    git config --gloval user.email <YOUR EMAIL HERE>

Assuming you have the most recent version of cFS (if not, see the README.md), if the master version you're working off of has been updated with commits, you can load those by using
    
    git pull

## Creating a branch

All the work we'll be doing will **always** be in a seperate branch, **never** on the master branch.
***Ever.***
The master branch will always be the most up-to-date build that fully compiles and works. So when developing, adding and editing code will first be done to a branch.

Creating a branch in git is easy, just use
    
    git branch <name of branch>

If you forget what brancg you made, or want a list, use
    
    git branch
Note! The asterisk shows you which branch you are currently working on. *It should never be the master branch.*
If you are on the master branch, switch with
    
    git switch <branch you're switching to>

Now go ahead and edit all the files you want, and follow the next section for pushing commits!

When your commits have been accepted and pulled into the master branch, delete your branch by
    
    git branch -d <name of branch being deleted>
the -d modifier ensures that the changes in the branch are already in the current branch (so you can run this command while in the master branch)
If you uber messed up your branch and want to delete it straight, use the -D modifier
    
    git branch -D <wrecked branch>


## Pushing Commits

If you've made changes to files, you can add them to temporary "staging area" by using
  
    git add <file name>

You can add multiple files by separating them with a space

This staging area allows you to set up all the files you want to commit , before you actually commit them

If you forget which files you've changed, git will tell you if you ask nicely:
    
    git status

Before actually commiting, see if any changes you've made are in direct conflict with
    
    git diff

When you're ready to commit, use the commit command
    
    git commit
It will prompt you for a message describing the change. **Always** add a message describing your changes.
If you want to do it all in one line, that's cool too
    
    git commit -m "Your change message here"

To actually push the commit to the Github, use
    
    git push
It will prompt you for a username and password (since our GitHub is organisation access only)
