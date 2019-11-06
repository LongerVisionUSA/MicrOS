#include "harddisk_ata.h"

uint32_t harddisk_ata_get_user_addressable_sectors(const harddisk_identify_device_data *data)
{
    return data->fields.total_number_of_user_addressable_sectors;
}

uint32_t harddisk_ata_get_disk_space(const harddisk_identify_device_data *data)
{
    // Multiply total number of user addressable sectors by number of bytes per sector (currently hard coded).
    return data->fields.total_number_of_user_addressable_sectors * 512;
}

int8_t harddisk_ata_read_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer)
{
    if(buffer == NULL) return -2;
    uint16_t io_port = 0;
    harddisk_io_drive_head_register message_to_drive = {.value = 0};

    // Set port of drive
    if (bus == HARDDISK_ATA_PRIMARY_BUS) io_port = HARDDISK_ATA_PRIMARY_BUS_IO_PORT;
    else if(bus == HARDDISK_ATA_SECONDARY_BUS) io_port = HARDDISK_ATA_SECONDARY_BUS_IO_PORT;
    else return -2;

    // Set drive
    switch (type)
    {
    case HARDDISK_ATA_MASTER:
        // For master set it to 0x40. Choose to use LBA.
        message_to_drive.fields.chs_head_lba_block_number = 1;
        break;
    case HARDDISK_ATA_SLAVE:
        // For slave set it to 0x50. Choose to use LBA and drive 1.
        message_to_drive.fields.chs_head_lba_block_number = 1;
        message_to_drive.fields.drive_number = 1;
        break;
    default:
        return -2;
    }

    // Send message to drive
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive.value);

    // Send what to read
    io_out_byte(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_byte(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, (uint8_t)high_lba);
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(high_lba >> 8));
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(high_lba >> 16));
    io_out_byte(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 1);
    io_out_byte(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, (uint8_t)low_lba);
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(low_lba >> 8));
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(low_lba >> 16));

    // Send the READ SECTORS EXT command to command register of I/O port.
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, HARDDISK_READ_SECTORS_EXT_COMMAND);

    __harddisk_400ns_delay(io_port);

    // For any other value: poll the Status port until bit 7 (BSY, value = 0x80) clears.
    int8_t pooling_result = __harddisk_ata_poll(io_port);
    if(pooling_result == 1)
    {
        for(int i = 0; i < 256; i++)
        {
            // Read 256 16-bit values, and store them.
            buffer[i] = io_in_word(io_port);
        }
        return 1;
    }
    else
    {
        // Error occured
        return -1;
    }

}

int8_t harddisk_ata_write_sector(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, uint32_t high_lba, uint32_t low_lba, uint16_t *buffer)
{
    if(buffer == NULL) return -2;
    uint16_t io_port = 0;
    harddisk_io_drive_head_register message_to_drive = {.value = 0};

    // Set port of drive
    if (bus == HARDDISK_ATA_PRIMARY_BUS) io_port = HARDDISK_ATA_PRIMARY_BUS_IO_PORT;
    else if(bus == HARDDISK_ATA_SECONDARY_BUS) io_port = HARDDISK_ATA_SECONDARY_BUS_IO_PORT;
    else return -2;

    // Set drive
    switch (type)
    {
    case HARDDISK_ATA_MASTER:
        // For master set it to 0x40. Choose to use LBA.
        message_to_drive.fields.chs_head_lba_block_number = 1;
        break;
    case HARDDISK_ATA_SLAVE:
        // For slave set it to 0x50. Choose to use LBA and drive 1.
        message_to_drive.fields.chs_head_lba_block_number = 1;
        message_to_drive.fields.drive_number = 1;
        break;
    default:
        return -2;
    }

    // Send message to drive
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive.value);

    // Send what to write
    io_out_byte(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_byte(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, (uint8_t)high_lba);
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(high_lba >> 8));
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(high_lba >> 16));
    io_out_byte(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 1);
    io_out_byte(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, (uint8_t)low_lba);
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, (uint8_t)(low_lba >> 8));
    io_out_byte(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, (uint8_t)(low_lba >> 16));

    // Send the WRITE SECTORS EXT command to command register of I/O port.
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, HARDDISK_WRITE_SECTORS_EXT_COMMAND);

    __harddisk_400ns_delay(io_port);

    // For any other value: poll the Status port until bit 7 (BSY, value = 0x80) clears.
    int8_t pooling_result = __harddisk_ata_poll(io_port);
    if(pooling_result == 1)
    {
        for(int i = 0; i < 256; i++)
        {
            // Write 256 16-bit values.
            io_out_word(io_port, buffer[i]);
            // Cache flush.
            io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, 0xE7);
        }
        return 1;
    }
    else
    {
        // Error occured
        return -1;
    }

}

int8_t __harddisk_ata_check_presence(HARDDISK_ATA_MASTER_SLAVE type, HARDDISK_ATA_BUS_TYPE bus, harddisk_identify_device_data *data)
{
    uint16_t io_port = 0;
    uint16_t control_port = 0;
    harddisk_io_drive_head_register message_to_drive = {.value = 0};

    // Set port of drive
    if (bus == HARDDISK_ATA_PRIMARY_BUS) {io_port = HARDDISK_ATA_PRIMARY_BUS_IO_PORT; control_port = HARDDISK_ATA_PRIMARY_BUS_CONTROL_PORT;}
    else if(bus == HARDDISK_ATA_SECONDARY_BUS) {io_port = HARDDISK_ATA_SECONDARY_BUS_IO_PORT; control_port = HARDDISK_ATA_SECONDARY_BUS_CONTROL_PORT;}
    else return -2;

    // Set drive
    switch (type)
    {
    case HARDDISK_ATA_MASTER:
        // For master set it to 0xA0. We set 2 bits that should be always 1.
        message_to_drive.fields.always_set_field_1 = 1;
        message_to_drive.fields.always_set_field_2 = 1;
        break;
    case HARDDISK_ATA_SLAVE:
        // For slave set it to 0xB0. We set 2 bits that should be always 1 and drive number to 1.
        message_to_drive.fields.always_set_field_1 = 1;
        message_to_drive.fields.always_set_field_2 = 1;
        message_to_drive.fields.drive_number = 1;
        break;
    default:
        return -2;
    }

    // Send message to drive
    io_out_byte(io_port + HARDDISK_IO_DRIVE_HEAD_REGISTER_OFFSET, message_to_drive.value);

    // Make 400ns delay
    __harddisk_400ns_delay(io_port);

    // Set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0
    io_out_word(io_port + HARDDISK_IO_SECTOR_COUNT_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_SECTOR_NUMBER_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_LOW_REGISTER_OFFSET, 0);
    io_out_word(io_port + HARDDISK_IO_CYLINDER_HIGH_REGISTER_OFFSET, 0);

    // Send the IDENTIFY command (0xEC) to the Command IO port.
    io_out_byte(io_port + HARDDISK_IO_COMMAND_REGISTER_OFFSET, HARDDISK_IDENTIFY_DEVICE_COMMAND);

    // Read the Status port again.
    harddisk_io_control_status_register result;
    result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);

    // If the value read is 0, the drive does not exist.
    if(result.value == 0)
    {
        return 0;
    }  
    else
    {   // For any other value: poll the Status port until bit 7 (BSY, value = 0x80) clears.
        for(;;)
        {
            if(result.fields.busy == 0)
            {
                // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
                for(;;)
                {
                    result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
                    if(result.fields.has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1)
                    {
                        for(int i = 0; i < 256; i++)
                        {
                            //  Read 256 16-bit values, and store them.
                            data->values[i] = io_in_word(io_port);
                        }
                        return 1;
                    }
                    else if(result.fields.error_occurred == 1)
                    {
                        return -1;
                    }
                }
            }   
            result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
        }
    }
}

int8_t __harddisk_ata_poll(uint16_t io_port)
{
    harddisk_io_control_status_register result;
    for(;;)
    {
        result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
        if(result.fields.busy == 0)
        {
            // Otherwise, continue polling one of the Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
            for(;;)
            {
                result.value = io_in_byte(io_port + HARDDISK_IO_STATUS_REGISTER_OFFSET);
                if(result.fields.has_pio_data_to_transfer_or_ready_to_accept_pio_data == 1)
                {
                    return 1;
                }
                else if(result.fields.error_occurred == 1)
                {
                    return -1;
                }
            }
        }
 
    }
}