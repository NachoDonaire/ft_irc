port=$1
#(printf "PASS"; sleep 1; printf " kaka \r\n";  sleep 1;
(printf "USER tukie 0 * :realname\r\n"; sleep 1; printf "NICK julio \r\n") | nc localhost $port
#(printf "JOIN #gulagula\r\n"; sleep 1; printf "NICK julio \r\n") | nc localhost 6667

