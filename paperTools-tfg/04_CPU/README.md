# CPU measurement

# Dependencies

```bash
sudo apt install -y linux-tools-generic linux-tools-common
``` 

# Useful pipes

```bash
 test/mpstat.2 | tail -n 84 | sed '/^[[:space:]]*$/d' | grep all | awk -F '  +' '{print $8}' | tr ',' '.' | tr '\n' ','
```
