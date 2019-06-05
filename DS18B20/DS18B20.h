#ifndef _DS18B20_H_
#define _DS18B20_H_

//DS18B20 commands
// clang-format off
#define DS18B20_READ_ROM      0x33
#define DS18B20_MATCH_ROM     0x55
#define DS18B20_SKIP_ROM      0xCC
#define DS18B20_SEARCH_ROM    0xF0
#define DS18B20_START_CONVERT 0x44
#define DS18B20_READ_SCRTPD   0xBE
#define DS18B20_WRITE_SCRTPD  0x4E
#define DS18B20_COPY_SCRTPD   0x48
#define DS18B20_SEARCH_ALARM  0xEC
#define DS18B20_RECALL_EE     0xB8
#define DS18B20_READ_POWER    0xB4
// clang-format on

/**
 * @brief initialize port and configure DS18B20
 */
unsigned char DS18B20Init(void);

/**
 * @brief convert temperature
 *
 * @return 1 for success, 0 for failed
 */
unsigned char DS18B20ConvertTemp(void);
/**
 * @brief Read scrtpd
 *
 * @return 1 for success, 0 for failed
 */
unsigned char DS18B20ReadSCRTPD(void);

/**
 * @brief get temperature
 *
 * @return 
 */
short int DS18B20GetTemp(void);
#endif /* ifndef _DS18B20_H_ */
