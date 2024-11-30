#!/bin/bash

# Get the directory of the script itself (assumed to be the repo folder)
REPO_PATH="$(dirname "$0")"

# Change to the repo directory
cd "$REPO_PATH"

# Monitor the repository for changes (e.g., modify, create files)
while inotifywait -r -e modify,create,delete "$REPO_PATH"; do
    # Add changes
    git add .

    # Commit changes (with a message that includes the current date and time)
    git commit -m "Auto commit on $(date)"

    # Push changes to GitHub using SSH
    git push origin master  # Replace 'main' with your branch name
done

