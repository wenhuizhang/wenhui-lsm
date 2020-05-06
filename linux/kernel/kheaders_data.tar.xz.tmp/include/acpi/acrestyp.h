/* SPDX-License-Identifier: BSD-3-Clause OR GPL-2.0 */


#ifndef __ACRESTYP_H__
#define __ACRESTYP_H__


typedef u16 acpi_rs_length;	
typedef u32 acpi_rsdesc_size;	


#define ACPI_READ_ONLY_MEMORY           (u8) 0x00
#define ACPI_READ_WRITE_MEMORY          (u8) 0x01

#define ACPI_NON_CACHEABLE_MEMORY       (u8) 0x00
#define ACPI_CACHABLE_MEMORY            (u8) 0x01
#define ACPI_WRITE_COMBINING_MEMORY     (u8) 0x02
#define ACPI_PREFETCHABLE_MEMORY        (u8) 0x03





#define ACPI_NON_ISA_ONLY_RANGES        (u8) 0x01
#define ACPI_ISA_ONLY_RANGES            (u8) 0x02
#define ACPI_ENTIRE_RANGE               (ACPI_NON_ISA_ONLY_RANGES | ACPI_ISA_ONLY_RANGES)



#define ACPI_SPARSE_TRANSLATION         (u8) 0x01


#define ACPI_DECODE_10                  (u8) 0x00	
#define ACPI_DECODE_16                  (u8) 0x01	





#define ACPI_LEVEL_SENSITIVE            (u8) 0x00
#define ACPI_EDGE_SENSITIVE             (u8) 0x01



#define ACPI_ACTIVE_HIGH                (u8) 0x00
#define ACPI_ACTIVE_LOW                 (u8) 0x01
#define ACPI_ACTIVE_BOTH                (u8) 0x02



#define ACPI_EXCLUSIVE                  (u8) 0x00
#define ACPI_SHARED                     (u8) 0x01



#define ACPI_NOT_WAKE_CAPABLE           (u8) 0x00
#define ACPI_WAKE_CAPABLE               (u8) 0x01


#define ACPI_COMPATIBILITY              (u8) 0x00
#define ACPI_TYPE_A                     (u8) 0x01
#define ACPI_TYPE_B                     (u8) 0x02
#define ACPI_TYPE_F                     (u8) 0x03

#define ACPI_NOT_BUS_MASTER             (u8) 0x00
#define ACPI_BUS_MASTER                 (u8) 0x01

#define ACPI_TRANSFER_8                 (u8) 0x00
#define ACPI_TRANSFER_8_16              (u8) 0x01
#define ACPI_TRANSFER_16                (u8) 0x02


#define ACPI_GOOD_CONFIGURATION         (u8) 0x00
#define ACPI_ACCEPTABLE_CONFIGURATION   (u8) 0x01
#define ACPI_SUB_OPTIMAL_CONFIGURATION  (u8) 0x02


#define ACPI_MEMORY_RANGE               (u8) 0x00
#define ACPI_IO_RANGE                   (u8) 0x01
#define ACPI_BUS_NUMBER_RANGE           (u8) 0x02

#define ACPI_ADDRESS_NOT_FIXED          (u8) 0x00
#define ACPI_ADDRESS_FIXED              (u8) 0x01

#define ACPI_POS_DECODE                 (u8) 0x00
#define ACPI_SUB_DECODE                 (u8) 0x01



#define ACPI_PRODUCER                   (u8) 0x00
#define ACPI_CONSUMER                   (u8) 0x01


#ifndef ACPI_MISALIGNMENT_NOT_SUPPORTED
#pragma pack(1)
#endif



struct acpi_uuid {
	u8 data[ACPI_UUID_LENGTH];
};

struct acpi_vendor_uuid {
	u8 subtype;
	u8 data[ACPI_UUID_LENGTH];
};


struct acpi_resource_irq {
	u8 descriptor_length;
	u8 triggering;
	u8 polarity;
	u8 shareable;
	u8 wake_capable;
	u8 interrupt_count;
	u8 interrupts[1];
};

struct acpi_resource_dma {
	u8 type;
	u8 bus_master;
	u8 transfer;
	u8 channel_count;
	u8 channels[1];
};

struct acpi_resource_start_dependent {
	u8 descriptor_length;
	u8 compatibility_priority;
	u8 performance_robustness;
};



struct acpi_resource_io {
	u8 io_decode;
	u8 alignment;
	u8 address_length;
	u16 minimum;
	u16 maximum;
};

struct acpi_resource_fixed_io {
	u16 address;
	u8 address_length;
};

struct acpi_resource_fixed_dma {
	u16 request_lines;
	u16 channels;
	u8 width;
};



#define ACPI_DMA_WIDTH8                         0
#define ACPI_DMA_WIDTH16                        1
#define ACPI_DMA_WIDTH32                        2
#define ACPI_DMA_WIDTH64                        3
#define ACPI_DMA_WIDTH128                       4
#define ACPI_DMA_WIDTH256                       5

struct acpi_resource_vendor {
	u16 byte_length;
	u8 byte_data[1];
};



struct acpi_resource_vendor_typed {
	u16 byte_length;
	u8 uuid_subtype;
	u8 uuid[ACPI_UUID_LENGTH];
	u8 byte_data[1];
};

struct acpi_resource_end_tag {
	u8 checksum;
};

struct acpi_resource_memory24 {
	u8 write_protect;
	u16 minimum;
	u16 maximum;
	u16 alignment;
	u16 address_length;
};

struct acpi_resource_memory32 {
	u8 write_protect;
	u32 minimum;
	u32 maximum;
	u32 alignment;
	u32 address_length;
};

struct acpi_resource_fixed_memory32 {
	u8 write_protect;
	u32 address;
	u32 address_length;
};

struct acpi_memory_attribute {
	u8 write_protect;
	u8 caching;
	u8 range_type;
	u8 translation;
};

struct acpi_io_attribute {
	u8 range_type;
	u8 translation;
	u8 translation_type;
	u8 reserved1;
};

union acpi_resource_attribute {
	struct acpi_memory_attribute mem;
	struct acpi_io_attribute io;

	

	u8 type_specific;
};

struct acpi_resource_label {
	u16 string_length;
	char *string_ptr;
};

struct acpi_resource_source {
	u8 index;
	u16 string_length;
	char *string_ptr;
};



#define ACPI_RESOURCE_ADDRESS_COMMON \
	u8                                      resource_type; \
	u8                                      producer_consumer; \
	u8                                      decode; \
	u8                                      min_address_fixed; \
	u8                                      max_address_fixed; \
	union acpi_resource_attribute           info;

struct acpi_address16_attribute {
	u16 granularity;
	u16 minimum;
	u16 maximum;
	u16 translation_offset;
	u16 address_length;
};

struct acpi_address32_attribute {
	u32 granularity;
	u32 minimum;
	u32 maximum;
	u32 translation_offset;
	u32 address_length;
};

struct acpi_address64_attribute {
	u64 granularity;
	u64 minimum;
	u64 maximum;
	u64 translation_offset;
	u64 address_length;
};

struct acpi_resource_address {
ACPI_RESOURCE_ADDRESS_COMMON};

struct acpi_resource_address16 {
	ACPI_RESOURCE_ADDRESS_COMMON struct acpi_address16_attribute address;
	struct acpi_resource_source resource_source;
};

struct acpi_resource_address32 {
	ACPI_RESOURCE_ADDRESS_COMMON struct acpi_address32_attribute address;
	struct acpi_resource_source resource_source;
};

struct acpi_resource_address64 {
	ACPI_RESOURCE_ADDRESS_COMMON struct acpi_address64_attribute address;
	struct acpi_resource_source resource_source;
};

struct acpi_resource_extended_address64 {
	ACPI_RESOURCE_ADDRESS_COMMON u8 revision_ID;
	struct acpi_address64_attribute address;
	u64 type_specific;
};

struct acpi_resource_extended_irq {
	u8 producer_consumer;
	u8 triggering;
	u8 polarity;
	u8 shareable;
	u8 wake_capable;
	u8 interrupt_count;
	struct acpi_resource_source resource_source;
	u32 interrupts[1];
};

struct acpi_resource_generic_register {
	u8 space_id;
	u8 bit_width;
	u8 bit_offset;
	u8 access_size;
	u64 address;
};

struct acpi_resource_gpio {
	u8 revision_id;
	u8 connection_type;
	u8 producer_consumer;	
	u8 pin_config;
	u8 shareable;		
	u8 wake_capable;	
	u8 io_restriction;
	u8 triggering;		
	u8 polarity;		
	u16 drive_strength;
	u16 debounce_timeout;
	u16 pin_table_length;
	u16 vendor_length;
	struct acpi_resource_source resource_source;
	u16 *pin_table;
	u8 *vendor_data;
};



#define ACPI_RESOURCE_GPIO_TYPE_INT             0
#define ACPI_RESOURCE_GPIO_TYPE_IO              1



#define ACPI_PIN_CONFIG_DEFAULT                 0
#define ACPI_PIN_CONFIG_PULLUP                  1
#define ACPI_PIN_CONFIG_PULLDOWN                2
#define ACPI_PIN_CONFIG_NOPULL                  3



#define ACPI_IO_RESTRICT_NONE                   0
#define ACPI_IO_RESTRICT_INPUT                  1
#define ACPI_IO_RESTRICT_OUTPUT                 2
#define ACPI_IO_RESTRICT_NONE_PRESERVE          3



#define ACPI_RESOURCE_SERIAL_COMMON \
	u8                                      revision_id; \
	u8                                      type; \
	u8                                      producer_consumer;   \
	u8                                      slave_mode; \
	u8                                      connection_sharing; \
	u8                                      type_revision_id; \
	u16                                     type_data_length; \
	u16                                     vendor_length; \
	struct acpi_resource_source             resource_source; \
	u8                                      *vendor_data;

struct acpi_resource_common_serialbus {
ACPI_RESOURCE_SERIAL_COMMON};



#define ACPI_RESOURCE_SERIAL_TYPE_I2C           1
#define ACPI_RESOURCE_SERIAL_TYPE_SPI           2
#define ACPI_RESOURCE_SERIAL_TYPE_UART          3



#define ACPI_CONTROLLER_INITIATED               0
#define ACPI_DEVICE_INITIATED                   1

struct acpi_resource_i2c_serialbus {
	ACPI_RESOURCE_SERIAL_COMMON u8 access_mode;
	u16 slave_address;
	u32 connection_speed;
};



#define ACPI_I2C_7BIT_MODE                      0
#define ACPI_I2C_10BIT_MODE                     1

struct acpi_resource_spi_serialbus {
	ACPI_RESOURCE_SERIAL_COMMON u8 wire_mode;
	u8 device_polarity;
	u8 data_bit_length;
	u8 clock_phase;
	u8 clock_polarity;
	u16 device_selection;
	u32 connection_speed;
};



#define ACPI_SPI_4WIRE_MODE                     0
#define ACPI_SPI_3WIRE_MODE                     1



#define ACPI_SPI_ACTIVE_LOW                     0
#define ACPI_SPI_ACTIVE_HIGH                    1



#define ACPI_SPI_FIRST_PHASE                    0
#define ACPI_SPI_SECOND_PHASE                   1



#define ACPI_SPI_START_LOW                      0
#define ACPI_SPI_START_HIGH                     1

struct acpi_resource_uart_serialbus {
	ACPI_RESOURCE_SERIAL_COMMON u8 endian;
	u8 data_bits;
	u8 stop_bits;
	u8 flow_control;
	u8 parity;
	u8 lines_enabled;
	u16 rx_fifo_size;
	u16 tx_fifo_size;
	u32 default_baud_rate;
};



#define ACPI_UART_LITTLE_ENDIAN                 0
#define ACPI_UART_BIG_ENDIAN                    1



#define ACPI_UART_5_DATA_BITS                   0
#define ACPI_UART_6_DATA_BITS                   1
#define ACPI_UART_7_DATA_BITS                   2
#define ACPI_UART_8_DATA_BITS                   3
#define ACPI_UART_9_DATA_BITS                   4



#define ACPI_UART_NO_STOP_BITS                  0
#define ACPI_UART_1_STOP_BIT                    1
#define ACPI_UART_1P5_STOP_BITS                 2
#define ACPI_UART_2_STOP_BITS                   3



#define ACPI_UART_FLOW_CONTROL_NONE             0
#define ACPI_UART_FLOW_CONTROL_HW               1
#define ACPI_UART_FLOW_CONTROL_XON_XOFF         2



#define ACPI_UART_PARITY_NONE                   0
#define ACPI_UART_PARITY_EVEN                   1
#define ACPI_UART_PARITY_ODD                    2
#define ACPI_UART_PARITY_MARK                   3
#define ACPI_UART_PARITY_SPACE                  4



#define ACPI_UART_CARRIER_DETECT                (1<<2)
#define ACPI_UART_RING_INDICATOR                (1<<3)
#define ACPI_UART_DATA_SET_READY                (1<<4)
#define ACPI_UART_DATA_TERMINAL_READY           (1<<5)
#define ACPI_UART_CLEAR_TO_SEND                 (1<<6)
#define ACPI_UART_REQUEST_TO_SEND               (1<<7)

struct acpi_resource_pin_function {
	u8 revision_id;
	u8 pin_config;
	u8 shareable;		
	u16 function_number;
	u16 pin_table_length;
	u16 vendor_length;
	struct acpi_resource_source resource_source;
	u16 *pin_table;
	u8 *vendor_data;
};

struct acpi_resource_pin_config {
	u8 revision_id;
	u8 producer_consumer;	
	u8 shareable;		
	u8 pin_config_type;
	u32 pin_config_value;
	u16 pin_table_length;
	u16 vendor_length;
	struct acpi_resource_source resource_source;
	u16 *pin_table;
	u8 *vendor_data;
};



#define ACPI_PIN_CONFIG_DEFAULT                 0
#define ACPI_PIN_CONFIG_BIAS_PULL_UP            1
#define ACPI_PIN_CONFIG_BIAS_PULL_DOWN          2
#define ACPI_PIN_CONFIG_BIAS_DEFAULT            3
#define ACPI_PIN_CONFIG_BIAS_DISABLE            4
#define ACPI_PIN_CONFIG_BIAS_HIGH_IMPEDANCE     5
#define ACPI_PIN_CONFIG_BIAS_BUS_HOLD           6
#define ACPI_PIN_CONFIG_DRIVE_OPEN_DRAIN        7
#define ACPI_PIN_CONFIG_DRIVE_OPEN_SOURCE       8
#define ACPI_PIN_CONFIG_DRIVE_PUSH_PULL         9
#define ACPI_PIN_CONFIG_DRIVE_STRENGTH          10
#define ACPI_PIN_CONFIG_SLEW_RATE               11
#define ACPI_PIN_CONFIG_INPUT_DEBOUNCE          12
#define ACPI_PIN_CONFIG_INPUT_SCHMITT_TRIGGER   13

struct acpi_resource_pin_group {
	u8 revision_id;
	u8 producer_consumer;	
	u16 pin_table_length;
	u16 vendor_length;
	u16 *pin_table;
	struct acpi_resource_label resource_label;
	u8 *vendor_data;
};

struct acpi_resource_pin_group_function {
	u8 revision_id;
	u8 producer_consumer;	
	u8 shareable;		
	u16 function_number;
	u16 vendor_length;
	struct acpi_resource_source resource_source;
	struct acpi_resource_label resource_source_label;
	u8 *vendor_data;
};

struct acpi_resource_pin_group_config {
	u8 revision_id;
	u8 producer_consumer;	
	u8 shareable;		
	u8 pin_config_type;	
	u32 pin_config_value;
	u16 vendor_length;
	struct acpi_resource_source resource_source;
	struct acpi_resource_label resource_source_label;
	u8 *vendor_data;
};



#define ACPI_RESOURCE_TYPE_IRQ                  0
#define ACPI_RESOURCE_TYPE_DMA                  1
#define ACPI_RESOURCE_TYPE_START_DEPENDENT      2
#define ACPI_RESOURCE_TYPE_END_DEPENDENT        3
#define ACPI_RESOURCE_TYPE_IO                   4
#define ACPI_RESOURCE_TYPE_FIXED_IO             5
#define ACPI_RESOURCE_TYPE_VENDOR               6
#define ACPI_RESOURCE_TYPE_END_TAG              7
#define ACPI_RESOURCE_TYPE_MEMORY24             8
#define ACPI_RESOURCE_TYPE_MEMORY32             9
#define ACPI_RESOURCE_TYPE_FIXED_MEMORY32       10
#define ACPI_RESOURCE_TYPE_ADDRESS16            11
#define ACPI_RESOURCE_TYPE_ADDRESS32            12
#define ACPI_RESOURCE_TYPE_ADDRESS64            13
#define ACPI_RESOURCE_TYPE_EXTENDED_ADDRESS64   14	
#define ACPI_RESOURCE_TYPE_EXTENDED_IRQ         15
#define ACPI_RESOURCE_TYPE_GENERIC_REGISTER     16
#define ACPI_RESOURCE_TYPE_GPIO                 17	
#define ACPI_RESOURCE_TYPE_FIXED_DMA            18	
#define ACPI_RESOURCE_TYPE_SERIAL_BUS           19	
#define ACPI_RESOURCE_TYPE_PIN_FUNCTION         20	
#define ACPI_RESOURCE_TYPE_PIN_CONFIG           21	
#define ACPI_RESOURCE_TYPE_PIN_GROUP            22	
#define ACPI_RESOURCE_TYPE_PIN_GROUP_FUNCTION   23	
#define ACPI_RESOURCE_TYPE_PIN_GROUP_CONFIG     24	
#define ACPI_RESOURCE_TYPE_MAX                  24



union acpi_resource_data {
	struct acpi_resource_irq irq;
	struct acpi_resource_dma dma;
	struct acpi_resource_start_dependent start_dpf;
	struct acpi_resource_io io;
	struct acpi_resource_fixed_io fixed_io;
	struct acpi_resource_fixed_dma fixed_dma;
	struct acpi_resource_vendor vendor;
	struct acpi_resource_vendor_typed vendor_typed;
	struct acpi_resource_end_tag end_tag;
	struct acpi_resource_memory24 memory24;
	struct acpi_resource_memory32 memory32;
	struct acpi_resource_fixed_memory32 fixed_memory32;
	struct acpi_resource_address16 address16;
	struct acpi_resource_address32 address32;
	struct acpi_resource_address64 address64;
	struct acpi_resource_extended_address64 ext_address64;
	struct acpi_resource_extended_irq extended_irq;
	struct acpi_resource_generic_register generic_reg;
	struct acpi_resource_gpio gpio;
	struct acpi_resource_i2c_serialbus i2c_serial_bus;
	struct acpi_resource_spi_serialbus spi_serial_bus;
	struct acpi_resource_uart_serialbus uart_serial_bus;
	struct acpi_resource_common_serialbus common_serial_bus;
	struct acpi_resource_pin_function pin_function;
	struct acpi_resource_pin_config pin_config;
	struct acpi_resource_pin_group pin_group;
	struct acpi_resource_pin_group_function pin_group_function;
	struct acpi_resource_pin_group_config pin_group_config;

	

	struct acpi_resource_address address;	
};



struct acpi_resource {
	u32 type;
	u32 length;
	union acpi_resource_data data;
};



#pragma pack()

#define ACPI_RS_SIZE_NO_DATA                8	
#define ACPI_RS_SIZE_MIN                    (u32) ACPI_ROUND_UP_TO_NATIVE_WORD (12)
#define ACPI_RS_SIZE(type)                  (u32) (ACPI_RS_SIZE_NO_DATA + sizeof (type))



#define ACPI_NEXT_RESOURCE(res) \
	ACPI_ADD_PTR (struct acpi_resource, (res), (res)->length)

struct acpi_pci_routing_table {
	u32 length;
	u32 pin;
	u64 address;		
	u32 source_index;
	char source[4];		
};

#endif				
