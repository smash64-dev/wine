/*
 * Wine Midi driver for Mac OS X
 *
 * Copyright 2006 Emmanuel Maillard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef WINE_COREMIDI_H
#define WINE_COREMIDI_H

#include <CoreFoundation/CoreFoundation.h>

#define MAX_MIDI_SYNTHS 1

#ifdef WINE_DEFINITIONS
/*
 * Due to CoreMIDI headers conflict redefine some types for Wine
 */
typedef void *MIDIClientRef;
typedef void *MIDIEndpointRef;
typedef void *MIDIPortRef;
typedef void *MIDIObjectRef;

typedef struct MIDIPacketList MIDIPacketList;

/*
 * functions
 */
extern OSStatus MIDIClientDispose(MIDIClientRef client);
extern unsigned MIDIGetNumberOfDestinations(void);
extern MIDIEndpointRef MIDIGetDestination(unsigned i);
extern unsigned MIDIGetNumberOfSources(void);
extern MIDIEndpointRef MIDIGetSource(unsigned i);
extern OSStatus MIDIOutputPortCreate(MIDIClientRef client, CFStringRef portName, MIDIPortRef *outPort);

typedef void (*MIDIReadProc)(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon);
extern OSStatus MIDIInputPortCreate(MIDIClientRef client, CFStringRef portName, MIDIReadProc readProc, void *refCon, MIDIPortRef *outPort);

extern OSStatus MIDIObjectGetProperties(MIDIObjectRef obj, CFPropertyListRef *outProperties, Boolean deep);

extern OSStatus MIDIPortConnectSource(MIDIPortRef port, MIDIEndpointRef source, void *connRefCon);

/*
 * Due to AudioUnit headers conflict redefine some types.
 */
typedef void *AudioUnit;
typedef void *AUGraph;

extern OSStatus MusicDeviceMIDIEvent(AudioUnit au, UInt32 inStatus, UInt32 inData1, UInt32 inData2, UInt32 inOffsetSampleFrame);
extern OSStatus MusicDeviceSysEx(AudioUnit au, const UInt8 *inData, UInt32 inLength);
#endif

/* midi.c */
typedef struct midi_src
{
    MIDIEndpointRef source;

    WORD wDevID;
    int state; /* 0 is no recording started, 1 in recording, bit 2 set if in sys exclusive recording */
    MIDIINCAPSW caps;
    MIDIOPENDESC midiDesc;
    LPMIDIHDR lpQueueHdr;
    WORD wFlags;
    DWORD startTime;
} MIDISource;

typedef struct {
    UInt16 devID;
    UInt16 length;
    Byte data[];
} MIDIMessage;

#endif
