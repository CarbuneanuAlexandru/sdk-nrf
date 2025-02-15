/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <lib/support/logging/CHIPLogging.h>

#include "app_task.h"
#include "lighting_manager.h"

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/util/af-types.h>
#include <app/util/af.h>

using namespace ::chip;
using namespace ::chip::app::Clusters;

void emberAfPostAttributeChangeCallback(EndpointId endpoint, ClusterId clusterId, AttributeId attributeId, uint8_t mask,
					uint16_t manufacturerCode, uint8_t type, uint16_t size, uint8_t *value)
{
	if (clusterId == OnOff::Id && attributeId == OnOff::Attributes::Ids::OnOff) {
		ChipLogProgress(Zcl, "Cluster OnOff: attribute OnOff set to %" PRIu8, *value);
		GetAppTask().PostEvent(AppEvent(*value ? AppEvent::On : AppEvent::Off, *value, true));
	} else if (clusterId == LevelControl::Id && attributeId == LevelControl::Attributes::Ids::CurrentLevel) {
		ChipLogProgress(Zcl, "Cluster LevelControl: attribute CurrentLevel set to %" PRIu8, *value);
		GetAppTask().PostEvent(AppEvent(AppEvent::Level, *value, true));
	}
}

/** @brief OnOff Cluster Init
 *
 * This function is called when a specific cluster is initialized. It gives the
 * application an opportunity to take care of cluster initialization procedures.
 * It is called exactly once for each endpoint where cluster is present.
 *
 * @param endpoint   Ver.: always
 *
 * TODO Issue #3841
 * emberAfOnOffClusterInitCallback happens before the stack initialize the cluster
 * attributes to the default value.
 * The logic here expects something similar to the deprecated Plugins callback
 * emberAfPluginOnOffClusterServerPostInitCallback.
 *
 */
void emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
	GetAppTask().UpdateClusterState();
}
