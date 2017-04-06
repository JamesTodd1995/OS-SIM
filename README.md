Make a commit each time you write new working code (about 10 lines) and each time you change existing code (about 5 lines).
# git commit -m "A short and descriptive message here"

Make a push each time you finish a new feature or programming task (about 1-5 commits).
# git push -u origin master

Make a pull every time you finish a task and are ready to accept other's changes. Alteratively, use the GitHub Desktop app, which notifies you when new changes have been made, and handles both pushing and pulling with a single "synchronize" button.
If you've already pushed your changes, 
# git pull 

but if you are in the middle of something and need somebody else's commit without overwriting your own,
# git pull --rebase

And if you encounter a merge conflict,
Log into github website to sort it out, then make more atomic commits in the future! Push more frequently and avoid working on the same thing at the same time as somebody else!

