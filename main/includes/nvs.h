#ifndef NVS_H_
#define NVS_H_

void nvs_init();
int32_t nvs_read_int(char *dir, char *key);
void nvs_write_int(char *dir, char *key, int32_t value);
char * nvs_read_str(char *dir, char *key, size_t *len);
void nvs_write_str(char *dir, char *key, char *value);

#endif /* NVS_H_ */
