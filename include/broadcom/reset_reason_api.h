/*
 *  BSD LICENSE
 *
 *  Copyright(c) 2015 Broadcom Corporation.  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *    * Neither the name of Broadcom Corporation nor the names of its
 *      contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RESET_REASON_API_H
#define RESET_REASON_API_H

struct reset_reason {
	uint32_t reason;
	uint32_t reserved;
};

enum {
	RESET_REASON_UNKNOWN             = 0x0,
	RESET_REASON_POWER_ON            = 0x1,
	RESET_REASON_PIN_RESET           = 0x2,
	RESET_REASON_WATCHDOG            = 0x3,
	RESET_REASON_SOFTWARE            = 0x4,
	RESET_REASON_WARM                = 0x5,
	RESET_REASON_WAKE_FROM_OFF       = 0x6,
	RESET_REASON_WAKE_FROM_SLEEP     = 0x7,
	RESET_REASON_WAKE_FROM_DEEPSLEEP = 0x8,
	RESET_REASON_MAX                 = 0x9
};

#endif /* RESET_REASON_API_H */
