/*
 * Copyright (C) 2017, Bin Meng <bmeng.cn@gmail.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <ahci.h>
#include <dm.h>
#include <pci.h>

static int ahci_pci_bind(struct udevice *dev)
{
	struct udevice *scsi_dev;
	int ret;

	ret = ahci_bind_scsi(dev, &scsi_dev);

	if (ret)
		goto out;

	scsi_dev->uclass_priv = calloc(1, sizeof(struct ahci_uc_priv));
	if (!scsi_dev->uclass_priv)
		ret = -ENOMEM;

out:
	return ret;
}

static int ahci_pci_probe(struct udevice *dev)
{
	return ahci_probe_scsi_pci(dev);
}

static const struct udevice_id ahci_pci_ids[] = {
	{ .compatible = "ahci-pci" },
	{ }
};

U_BOOT_DRIVER(ahci_pci) = {
	.name	= "ahci_pci",
	.id	= UCLASS_AHCI,
	.of_match = ahci_pci_ids,
	.bind	= ahci_pci_bind,
	.probe = ahci_pci_probe,
};

static struct pci_device_id ahci_pci_supported[] = {
	{ PCI_DEVICE_CLASS(PCI_CLASS_STORAGE_SATA_AHCI, ~0) },
	{},
};

U_BOOT_PCI_DEVICE(ahci_pci, ahci_pci_supported);
