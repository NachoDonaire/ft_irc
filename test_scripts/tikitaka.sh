port=$1
(printf "PASS"; sleep 1; printf " kaka \r\n"; sleep 1; printf "NICK julio \r\n"  sleep 1; printf "USER tukie 0 * :realname\r\n";sleep 1; printf "QU"; sleep 1; printf "IT" ; sleep 1; printf " julio\r\n")| nc localhost $port
