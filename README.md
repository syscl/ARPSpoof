# ARPSpoof
A C++ class aims at providing easy to use and clean arp spoofing 

# How to use?
- Download the latest ARPSpoof by entering the following command in a terminal window:
```sh
git clone https://github.com/syscl/ARPSpoof
```

- This will download the whole project to your current directory, jump right into the folder you just downloaded and generate the program you want by:
```sh
cd ARPSpoof
make
```

- If the compiled successfully, you can use it by the following:
```sh
request [routerIP] [victimIP] [replay]
```
or 
```sh
reply [routerIP] [victimIP] [replay]
```

- Note, the argument ```reply``` indicates how many packets you want to send, give a negative number will result infinity packets flooding out.

