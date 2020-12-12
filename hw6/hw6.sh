#!/bin/bash
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# hw6.sh - a small utility implementing the Ceaser Cipher #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
#
# Constants
declare -r red='\033[0;41;30m'
declare -r std='\033[0;0;39m'

# Array mapping translation char sets to rotation number == arr_idx
declare -ar tr_rot=('[A-Za-z]'       '[B-ZAb-za]'     '[C-ZA-Bc-za-b]'     # 0  1  2
                    '[D-ZA-Cd-za-c]' '[E-ZA-De-za-d]' '[F-ZA-Ef-za-e]'     # 3  4  5
                    '[G-ZA-Fg-za-f]' '[H-ZA-Gh-za-g]' '[I-ZA-Hi-za-h]'     # 6  7  8
                    '[J-ZA-Ij-za-i]' '[K-ZA-Jk-za-j]' '[L-ZA-Kl-za-k]'     # 9  10 11
                    '[M-ZA-Lm-za-l]' '[N-ZA-Mn-za-m]')                     # 12 13

# Global variables
declare out_file='./hw6.out'    # output save to this filename

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Enciphers text with user-chosen rotation
# Enciphers text with user-chosen rotation
# Globals:
#   out_file
# Args:
#   $1 is rot_num
#   $2 is optional input file path
# Outputs:
#   Various interactive prompting
# Returns:
#   true to restart user interaction
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
function enc() {
    if [[ -z "$2" ]]; then
        local plain_text
        read -p "Please type some text to encipher with ROT-$1: " plain_text
        echo ${plain_text} | tr "${tr_rot[0]}" "${tr_rot[$1]}" 2>/dev/null | tee ${out_file} \
            || echo -e "${red}Error occured during enciphering, please try again.${std}"
        read -p 'Press a key to continue...'
    else
        tr "${tr_rot[0]}" "${tr_rot[$1]}" < "$2" 2>/dev/null | tee ${out_file} \
            || echo -e "${red}Error occured during enciphering, please try again.${std}"
        read -p 'Press a key to continue...'
    fi
}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Deciphers text with user-chosen rotation
# Globals:
#   out_file
# Args:
#   $1 is rot_num
#   $2 is optional input file path
# Outputs:
#   Various interactive prompting
# Returns:
#   true to restart user interaction
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
function dec() {
    if [[ -z "$2" ]]; then
        local plain_text
        read -p "Please type some text to decipher with ROT-$1: " plain_text
        echo ${plain_text} | tr "${tr_rot[$1]}" "${tr_rot[0]}" | tee ${out_file} | tee ${out_file} \
            || echo -e "${red}Error occured during deciphering, please try again.${std}"
        read -p 'Press a key to continue...'
    else
        tr "${tr_rot[$1]}" "${tr_rot[0]}" < "$2" | tee ${out_file} | tee ${out_file} \
            || echo -e "${red}Error occured during deciphering, please try again.${std}"
        read -p 'Press a key to continue...'
    fi
}

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# Get all the user input via menuing system
# Globals:
#   out_file
# Args:
#   None
# Outputs:
#   Various interactive prompting
# Returns:
#   true to restart user interaction
#   false to exit script
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
user_input() {
    clear
    echo '+---------=---------+'
    echo ': M A I N - M E N U :'
    echo '+----=---------=----+'
    echo ''
    echo ' Will you provide'
    echo ' from a file or'
    echo ' type in/paste?'
    echo ''
    echo ' 1. Input from a file'
    echo ' 2. Enter text manually'
    echo ' 3. Exit'
    echo ''
    local choice    # Note: variable reused in multiple contexts
    read -p "Enter choice [ 1 - 3] " choice

    case ${choice} in
        1)
            declare in_file=''

            while true; do
                echo ''
                read -p 'Please enter the path to your input file: ' in_file

                if cat ${in_file} > /dev/null ; then
                    break 1
                else
                    echo -e "${red}Could not open the file ${in_file} for reading, try again.${std}"
                    echo 'Current directory contains these files:'
                    echo ''
                    ls -FAbl .
                fi
            done
        ;;
        2)  echo 'Manual entry chosen.' ; sleep 2
        ;;
        3)  exit 0
        ;;
        *) echo -e "${red}Please choose a number between one and three...${std}" ; sleep 2 ; return true
    esac

    echo ' What cipher are you using?'
    echo ''
    echo '   1. ROT-13'
    echo '   2. A Ceaser Cipher'
    echo '   3. Exit'
    echo ''
    read -p "Enter choice [ 1 - 3 ]: " choice

    local -i rot_num=0            # rotate this many letters
    case ${choice} in
        1) rot_num=13
        ;;
        2) 
            while true; do
                echo ''
                read -p 'How many rotations in your Ceaser Cipher [ 1 - 13 ]? ' rot_num

                if (( rot_num < 14 && rot_num > 0 )); then
                    break 1
                else
                    echo -e "${red}Please enter a number between one and thirteen.${std}"
                fi
            done
        ;;
        3) exit 0
        ;;
        *) echo -e "${red}Please choose a number between one and three...${std}" ; sleep 2 ; return true
    esac

    while true; do
        echo 'Are you enciphering or deciphering?'
        echo ''
        echo ' 1. Enciphering'
        echo ' 2. Deciphering'
        echo ' 3. In fact, let me exit'
        echo ''
        read -p 'Enter choice [ 1, 2, or 3 ]: ' choice
        ((choice == 3)) && exit 0

        ((choice < 5 && choice > 0)) \
            && break 1 \
            || echo -e "${red}Please choose one, two, or three.${std}"; continue 
    done

    local temp_file=''
    echo 'Please enter an output filename'
    echo "Default is ${out_file}"

    while true; do
        echo ''
        read -p 'Please enter filename, or just press Enter to accept the default: ' temp_file
        [[ "${temp_file}" == '' ]] && temp_file="${out_file}"

        # sanity check the user's filename to ensure we can write to it
        if touch ${temp_file} >/dev/null 2>&1 ; then
            out_file=${temp_file}; break 1
        else
            echo -e "${red}There was a problem creating the output file, please try again.${std}"; sleep 2 ; continue 
        fi
    done

    case $choice in
        1) enc ${rot_num} ${in_file}
        ;;
        2) dec ${rot_num} ${in_file}
        ;;
    esac
    #DEBUG
    echo "rot_num: ${rot_num}"
}
 
# main 
while true; do
    user_input
done

exit 0

# EOF
# vim: expandtab:smartindent:ts=4:sw=4
