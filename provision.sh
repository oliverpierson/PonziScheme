wget http://gnu.askapache.com/bison/bison-2.3.tar.gz
tar xzf bison-2.3.tar.gz
cd bison-2.3
./configure
make 
make install

apt-get install -y g++ flex libboost-all-dev
