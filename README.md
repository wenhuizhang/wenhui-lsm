# wenhui-lsm

## 1. Disable Secure Boot

## 2. Compile Kernel

```
cd linux/

sudo apt-get build-dep linux-image-$(uname -r)
sudo apt-get install make cmake libncurses5-dev flex bison libssl-dev dkms libelf-dev git


cp ./config/config  .config

make mrproper
make

sudo make modules_install
sudo make install
```


## 3. Update Grub and Reboot
```
sudo update-grub
sudo update-grub2

// update grub pointer to the correct stub
sudo vim /boot/grub/grub.cfg 
sudo vim /etc/default/grub

sudo reboot
```


## 4. Check 

Then the image should be today's date
```
uname -a
```

### 5. Test commit 

Commit with Title and Description

'''
git commit -m "Title" -m "Description .........."
'''

