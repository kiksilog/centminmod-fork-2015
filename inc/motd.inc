setupmotd() {

ENABLEMOTD='n'

MOTDCHECK=$(cat /etc/motd)
CSFWCHECK=$(grep 'CSF' /etc/motd)
IPTABLEWCHECK=$(grep 'iptables -F' /etc/motd)
CMWCHECK=$(grep 'centminmod.com' /etc/motd)

	if [[ "$ENABLEMOTD" = [yY] ]]; then

if [[ -z "$MOTDCHECK" ]]; then

#echo "Backup /etc/motd"
#\cp -af /etc/motd /etc/motd-backup_$DT

cat > "/etc/motd" <<EOF
################################################################
# ! This server maybe running CSF Firewall !  
#   DO NOT run the below command or you 
#   will lock yourself out of the server: 
#
#   iptables -F 
#

EOF

elif  [[ ! -z "$MOTDCHECK" && -z "$CSFWCHECK" || -z "$IPTABLEWCHECK" || -z "$CMWCHECK" ]]; then

#echo "Backup /etc/motd"
\cp -af /etc/motd /etc/motd-backup_$DT

cat >> "/etc/motd" <<EOF
################################################################
# ! This server maybe running CSF Firewall !  
#   DO NOT run the below command or you 
#   will lock yourself out of the server: 
#
#   iptables -F 
#

EOF

fi
	fi #ENABLEMOTD

}