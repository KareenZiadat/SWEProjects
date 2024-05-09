#!/bin/bash

# Function to run the server
run_server() {
    read -p "Enter the port number: " port
    gcc client-server.c -o client-server.o -lm
    ./client-server.o localhost "$port"
}

# Function to search a file
search_file() {
    filename="loans.txt"  
    
    # Prompt the user for a search term
    read -p "Enter client's name: " search_term

    # Search for the entered term in the file using grep
    echo "Search results for '$search_term' in $filename:"
    grep -i "$search_term" "$filename"
}

# Function to change file permissions
change_permissions() {
    read -p "Enter the permission in numerical format (e.g., 700): " permission
    if [[ "$permission" =~ ^[0-7]{3}$ ]]; then
        chmod "$permission" "loans.txt"
        echo "Permission changed to $permission for loans.txt"
    else
        echo "Invalid permission format. Please enter a three-digit numerical permission."
    fi
}

# Main menu
echo "Choose an option:"
echo "1) Start the mortgage system"
echo "2) Search client information"
echo "3) Change permissions for loans.txt"
read -p "Enter your choice: " choice

if [ "$choice" == "1" ]; then
    run_server
elif [ "$choice" == "2" ]; then
    search_file
elif [ "$choice" == "3" ]; then
    change_permissions
else
    echo "Invalid choice. Please enter 1, 2, or 3."
fi
