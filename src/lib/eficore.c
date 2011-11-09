/*
    eficore.c 

    This code was taken from the source code for GRUB (where it was used
    for debugging purposes) and modified to suit needs of uefivars
*/
/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <string.h>
#include "grubefi.h"

void
efi_print_device_path(grub_efi_device_path_t *dp)
{
  while (1) {
      grub_efi_uint8_t type = GRUB_EFI_DEVICE_PATH_TYPE (dp);
      grub_efi_uint8_t subtype = GRUB_EFI_DEVICE_PATH_SUBTYPE (dp);
      grub_efi_uint16_t len = GRUB_EFI_DEVICE_PATH_LENGTH (dp);

      switch (type) {
	case GRUB_EFI_END_DEVICE_PATH_TYPE:
	  switch (subtype) {
	    case GRUB_EFI_END_ENTIRE_DEVICE_PATH_SUBTYPE:
	      printf ("\n");
	      break;
	    case GRUB_EFI_END_THIS_DEVICE_PATH_SUBTYPE:
	      printf ("\n");
	      break;
	    default:
	      printf ("/EndUnknown(%x)\n", (unsigned) subtype);
	      break;
	    }
	  break;

	case GRUB_EFI_HARDWARE_DEVICE_PATH_TYPE:
	  switch (subtype) {
	    case GRUB_EFI_PCI_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_pci_device_path_t pci;
		memcpy (&pci, dp, len);
		printf ("/PCI(%x,%x)",
			     (unsigned) pci.function, (unsigned) pci.device);
	      }
	      break;
	    case GRUB_EFI_PCCARD_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_pccard_device_path_t pccard;
		memcpy (&pccard, dp, len);
		printf ("/PCCARD(%x)",
			     (unsigned) pccard.function);
	      }
	      break;
	    case GRUB_EFI_MEMORY_MAPPED_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_memory_mapped_device_path_t mmapped;
		memcpy (&mmapped, dp, len);
		printf ("/MMap(%x,%llx,%llx)",
			     (unsigned) mmapped.memory_type,
			     mmapped.start_address,
			     mmapped.end_address);
	      }
	      break;
	    case GRUB_EFI_VENDOR_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_vendor_device_path_t vendor;
		memcpy (&vendor, dp, sizeof (vendor));
		printf ("/Vendor(%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x)",
			     (unsigned) vendor.vendor_guid.data1,
			     (unsigned) vendor.vendor_guid.data2,
			     (unsigned) vendor.vendor_guid.data3,
			     (unsigned) vendor.vendor_guid.data4[0],
			     (unsigned) vendor.vendor_guid.data4[1],
			     (unsigned) vendor.vendor_guid.data4[2],
			     (unsigned) vendor.vendor_guid.data4[3],
			     (unsigned) vendor.vendor_guid.data4[4],
			     (unsigned) vendor.vendor_guid.data4[5],
			     (unsigned) vendor.vendor_guid.data4[6],
			     (unsigned) vendor.vendor_guid.data4[7]);
	      }
	      break;
	    case GRUB_EFI_CONTROLLER_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_controller_device_path_t controller;
		memcpy (&controller, dp, len);
		printf ("/Ctrl(%x)",
			     (unsigned) controller.controller_number);
	      }
	      break;
	    default:
	      printf ("/UnknownHW(%x)", (unsigned) subtype);
	      break;
	    }
	  break;

	case GRUB_EFI_ACPI_DEVICE_PATH_TYPE:
	  switch (subtype) {
	    case GRUB_EFI_ACPI_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_acpi_device_path_t acpi;
		memcpy (&acpi, dp, len);
		printf ("/ACPI(%x,%x)", (unsigned) acpi.hid, (unsigned) acpi.uid);
	      }
	      break;
	    case GRUB_EFI_EXPANDED_ACPI_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_expanded_acpi_device_path_t eacpi;
		memcpy (&eacpi, dp, sizeof (eacpi));
		printf ("/ACPI(");

		if (GRUB_EFI_EXPANDED_ACPI_HIDSTR (dp)[0] == '\0')
		  printf ("%x,", (unsigned) eacpi.hid);
		else
		  printf ("%s,", GRUB_EFI_EXPANDED_ACPI_HIDSTR (dp));

		if (GRUB_EFI_EXPANDED_ACPI_UIDSTR (dp)[0] == '\0')
		  printf ("%x,", (unsigned) eacpi.uid);
		else
		  printf ("%s,", GRUB_EFI_EXPANDED_ACPI_UIDSTR (dp));

		if (GRUB_EFI_EXPANDED_ACPI_CIDSTR (dp)[0] == '\0')
		  printf ("%x)", (unsigned) eacpi.cid);
		else
		  printf ("%s)", GRUB_EFI_EXPANDED_ACPI_CIDSTR (dp));
	      }
	      break;
	    default:
	      printf ("/UnknownACPI(%x)", (unsigned) subtype);
	      break;
	    }
	  break;

	case GRUB_EFI_MESSAGING_DEVICE_PATH_TYPE:
	  switch (subtype)
	    {
	    case GRUB_EFI_ATAPI_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_atapi_device_path_t atapi;
		memcpy (&atapi, dp, len);
		printf ("/ATAPI(%x,%x,%x)",
			     (unsigned) atapi.primary_secondary,
			     (unsigned) atapi.slave_master,
			     (unsigned) atapi.lun);
	      }
	      break;
	    case GRUB_EFI_SCSI_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_scsi_device_path_t scsi;
		memcpy (&scsi, dp, len);
		printf ("/SCSI(%x,%x)",
			     (unsigned) scsi.pun,
			     (unsigned) scsi.lun);
	      }
	      break;
	    case GRUB_EFI_FIBRE_CHANNEL_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_fibre_channel_device_path_t fc;
		memcpy (&fc, dp, len);
		printf ("/FibreChannel(%llx,%llx)",
			     fc.wwn, fc.lun);
	      }
	      break;
	    case GRUB_EFI_1394_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_1394_device_path_t firewire;
		memcpy (&firewire, dp, len);
		printf ("/1394(%llx)", firewire.guid);
	      }
	      break;
	    case GRUB_EFI_USB_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_usb_device_path_t usb;
		memcpy (&usb, dp, len);
		printf ("/USB(%x,%x)",
			     (unsigned) usb.parent_port_number,
			     (unsigned) usb.interface);
	      }
	      break;
	    case GRUB_EFI_USB_CLASS_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_usb_class_device_path_t usb_class;
		memcpy (&usb_class, dp, len);
		printf ("/USBClass(%x,%x,%x,%x,%x)",
			     (unsigned) usb_class.vendor_id,
			     (unsigned) usb_class.product_id,
			     (unsigned) usb_class.device_class,
			     (unsigned) usb_class.device_subclass,
			     (unsigned) usb_class.device_protocol);
	      }
	      break;
	    case GRUB_EFI_I2O_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_i2o_device_path_t i2o;
		memcpy (&i2o, dp, len);
		printf ("/I2O(%x)", (unsigned) i2o.tid);
	      }
	      break;
	    case GRUB_EFI_MAC_ADDRESS_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_mac_address_device_path_t mac;
		memcpy (&mac, dp, len);
		printf ("/MacAddr(%x:%x:%x:%x:%x:%x,%x)",
			     (unsigned) mac.mac_address[0],
			     (unsigned) mac.mac_address[1],
			     (unsigned) mac.mac_address[2],
			     (unsigned) mac.mac_address[3],
			     (unsigned) mac.mac_address[4],
			     (unsigned) mac.mac_address[5],
			     (unsigned) mac.if_type);
	      }
	      break;
	    case GRUB_EFI_IPV4_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_ipv4_device_path_t ipv4;
		memcpy (&ipv4, dp, len);
		printf ("/IPv4(%u.%u.%u.%u,%u.%u.%u.%u,%u,%u,%x,%x)",
			     (unsigned) ipv4.local_ip_address[0],
			     (unsigned) ipv4.local_ip_address[1],
			     (unsigned) ipv4.local_ip_address[2],
			     (unsigned) ipv4.local_ip_address[3],
			     (unsigned) ipv4.remote_ip_address[0],
			     (unsigned) ipv4.remote_ip_address[1],
			     (unsigned) ipv4.remote_ip_address[2],
			     (unsigned) ipv4.remote_ip_address[3],
			     (unsigned) ipv4.local_port,
			     (unsigned) ipv4.remote_port,
			     (unsigned) ipv4.protocol,
			     (unsigned) ipv4.static_ip_address);
	      }
	      break;
	    case GRUB_EFI_IPV6_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_ipv6_device_path_t ipv6;
		memcpy (&ipv6, dp, len);
		printf ("/IPv6(%x:%x:%x:%x:%x:%x:%x:%x,%x:%x:%x:%x:%x:%x:%x:%x,%u,%u,%x,%x)",
			     (unsigned) ipv6.local_ip_address[0],
			     (unsigned) ipv6.local_ip_address[1],
			     (unsigned) ipv6.local_ip_address[2],
			     (unsigned) ipv6.local_ip_address[3],
			     (unsigned) ipv6.local_ip_address[4],
			     (unsigned) ipv6.local_ip_address[5],
			     (unsigned) ipv6.local_ip_address[6],
			     (unsigned) ipv6.local_ip_address[7],
			     (unsigned) ipv6.remote_ip_address[0],
			     (unsigned) ipv6.remote_ip_address[1],
			     (unsigned) ipv6.remote_ip_address[2],
			     (unsigned) ipv6.remote_ip_address[3],
			     (unsigned) ipv6.remote_ip_address[4],
			     (unsigned) ipv6.remote_ip_address[5],
			     (unsigned) ipv6.remote_ip_address[6],
			     (unsigned) ipv6.remote_ip_address[7],
			     (unsigned) ipv6.local_port,
			     (unsigned) ipv6.remote_port,
			     (unsigned) ipv6.protocol,
			     (unsigned) ipv6.static_ip_address);
	      }
	      break;
	    case GRUB_EFI_INFINIBAND_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_infiniband_device_path_t ib;
		memcpy (&ib, dp, len);
		printf ("/InfiniBand(%x,%llx,%llx,%llx)",
			     (unsigned) ib.port_gid[0], /* XXX */
			     ib.remote_id,
			     ib.target_port_id,
			     ib.device_id);
	      }
	      break;
	    case GRUB_EFI_UART_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_uart_device_path_t uart;
		memcpy (&uart, dp, len);
		printf ("/UART(%llu,%u,%x,%x)",
			     uart.baud_rate,
			     uart.data_bits,
			     uart.parity,
			     uart.stop_bits);
	      }
	      break;
	    case GRUB_EFI_VENDOR_MESSAGING_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_vendor_messaging_device_path_t vendor;
		memcpy (&vendor, dp, sizeof (vendor));
		printf ("/Vendor(%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x)",
			     (unsigned) vendor.vendor_guid.data1,
			     (unsigned) vendor.vendor_guid.data2,
			     (unsigned) vendor.vendor_guid.data3,
			     (unsigned) vendor.vendor_guid.data4[0],
			     (unsigned) vendor.vendor_guid.data4[1],
			     (unsigned) vendor.vendor_guid.data4[2],
			     (unsigned) vendor.vendor_guid.data4[3],
			     (unsigned) vendor.vendor_guid.data4[4],
			     (unsigned) vendor.vendor_guid.data4[5],
			     (unsigned) vendor.vendor_guid.data4[6],
			     (unsigned) vendor.vendor_guid.data4[7]);
	      }
	      break;
	    default:
	      printf ("/UnknownMessaging(%x)", (unsigned) subtype);
	      break;
	    }
	  break;

	case GRUB_EFI_MEDIA_DEVICE_PATH_TYPE:
	  switch (subtype)
	    {
	    case GRUB_EFI_HARD_DRIVE_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_hard_drive_device_path_t hd;
		memcpy (&hd, dp, len);
		printf ("/HD(%u,%llx,%llx,%02x%02x%02x%02x%02x%02x%02x%02x,%x,%x)",
			     hd.partition_number,
			     hd.partition_start,
			     hd.partition_size,
			     (unsigned) hd.partition_signature[0],
			     (unsigned) hd.partition_signature[1],
			     (unsigned) hd.partition_signature[2],
			     (unsigned) hd.partition_signature[3],
			     (unsigned) hd.partition_signature[4],
			     (unsigned) hd.partition_signature[5],
			     (unsigned) hd.partition_signature[6],
			     (unsigned) hd.partition_signature[7],
			     (unsigned) hd.mbr_type,
			     (unsigned) hd.signature_type);
	      }
	      break;
	    case GRUB_EFI_CDROM_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_cdrom_device_path_t cd;
		memcpy (&cd, dp, len);
		printf ("/CD(%u,%llx,%llx)",
			     cd.boot_entry,
			     cd.partition_start,
			     cd.partition_size);
	      }
	      break;
	    case GRUB_EFI_VENDOR_MEDIA_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_vendor_media_device_path_t vendor;
		memcpy (&vendor, dp, sizeof (vendor));
		printf ("/Vendor(%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x)",
			     (unsigned) vendor.vendor_guid.data1,
			     (unsigned) vendor.vendor_guid.data2,
			     (unsigned) vendor.vendor_guid.data3,
			     (unsigned) vendor.vendor_guid.data4[0],
			     (unsigned) vendor.vendor_guid.data4[1],
			     (unsigned) vendor.vendor_guid.data4[2],
			     (unsigned) vendor.vendor_guid.data4[3],
			     (unsigned) vendor.vendor_guid.data4[4],
			     (unsigned) vendor.vendor_guid.data4[5],
			     (unsigned) vendor.vendor_guid.data4[6],
			     (unsigned) vendor.vendor_guid.data4[7]);
	      }
	      break;
	    case GRUB_EFI_FILE_PATH_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_file_path_device_path_t *fp;
		grub_uint8_t buf[(len - 4) * 2 + 1];
		fp = (grub_efi_file_path_device_path_t *) dp;
#if FPM
		*grub_utf16_to_utf8 (buf, fp->path_name,
				     (len - 4) / sizeof (grub_efi_char16_t))
		  = '\0';
#endif
		printf ("/File(%s) FPM ERROR", buf);
	      }
	      break;
	    case GRUB_EFI_PROTOCOL_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_protocol_device_path_t proto;
		memcpy (&proto, dp, sizeof (proto));
		printf ("/Protocol(%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x)",
			     (unsigned) proto.guid.data1,
			     (unsigned) proto.guid.data2,
			     (unsigned) proto.guid.data3,
			     (unsigned) proto.guid.data4[0],
			     (unsigned) proto.guid.data4[1],
			     (unsigned) proto.guid.data4[2],
			     (unsigned) proto.guid.data4[3],
			     (unsigned) proto.guid.data4[4],
			     (unsigned) proto.guid.data4[5],
			     (unsigned) proto.guid.data4[6],
			     (unsigned) proto.guid.data4[7]);
	      }
	      break;
	    default:
	      printf ("/UnknownMedia(%x)", (unsigned) subtype);
	      break;
	    }
	  break;

	case GRUB_EFI_BIOS_DEVICE_PATH_TYPE:
	  switch (subtype)
	    {
	    case GRUB_EFI_BIOS_DEVICE_PATH_SUBTYPE:
	      {
		grub_efi_bios_device_path_t bios;
		memcpy (&bios, dp, sizeof (bios));
		printf ("/BIOS(%x,%x,%s)",
			     (unsigned) bios.device_type,
			     (unsigned) bios.status_flags,
			     (char *) (dp + 1));
	      }
	      break;
	    default:
	      printf ("/UnknownBIOS(%x)", (unsigned) subtype);
	      break;
	    }
	  break;

	default:
	  printf ("/UnknownType(%x,%x)\n",
		       (unsigned) type,
		       (unsigned) subtype);
	  return;
	  break;
	}

      if (GRUB_EFI_END_ENTIRE_DEVICE_PATH (dp))
	break;

      dp = (grub_efi_device_path_t *) ((char *) dp + len);
    }
}
