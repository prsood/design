rm -f shell >/dev/null
rm -f ctrl >/dev/null
gcc reverse_udp.c -O3 -lpthread -o shell 
gcc udp-client.c -O3 -lpthread -o ctrl

