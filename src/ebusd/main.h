/*
 * ebusd - daemon for communication with eBUS heating systems.
 * Copyright (C) 2014-2021 John Baier <ebusd@ebusd.eu>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EBUSD_MAIN_H_
#define EBUSD_MAIN_H_

#include <stdint.h>
#include <string>
#include <map>
#include "lib/ebus/data.h"
#include "lib/ebus/message.h"
#include "lib/ebus/result.h"
#include "lib/utils/log.h"

namespace ebusd {

/** \file ebusd/main.h
 * The main entry method doing all the startup handling.
 */

/** A structure holding all program options. */
struct options {
  const char* device;  //!< eBUS device (serial device or [udp:]ip:port) [/dev/ttyUSB0]
  bool noDeviceCheck;  //!< skip serial eBUS device test
  bool readOnly;  //!< read-only access to the device
  bool initialSend;  //!< send an initial escape symbol after connecting device
  unsigned int extraLatency;  //!< extra transfer latency in ms [0 for USB, 10 for IP]

  const char* configPath;  //!< path to CSV configuration files [http://ebusd.eu/config/]
  const char* overridePath;  //!< path to CSV configuration files to override the main files with
  bool scanConfig;  //!< pick configuration files matching initial scan
  /** the initial address to scan for scanconfig
   * (@a ESC=none, 0xfe=broadcast ident, @a SYN=full scan, else: single slave address). */
  symbol_t initialScan;
  const char* preferLanguage;  //!< preferred language in configuration files
  bool checkConfig;  //!< check config files, then stop
  OutputFormat dumpConfig;   //!< dump config files, then stop
  unsigned int pollInterval;  //!< poll interval in seconds, 0 to disable [5]
  bool injectMessages;  //!< inject remaining arguments as already seen messages

  symbol_t address;  //!< own bus address [31]
  bool answer;  //!< answer to requests from other masters
  unsigned int acquireTimeout;  //!< bus acquisition timeout in ms [10]
  unsigned int acquireRetries;  //!< number of retries for bus acquisition [3]
  unsigned int sendRetries;  //!< number of retries for failed sends [2]
  unsigned int receiveTimeout;  //!< timeout for receiving answer from slave in ms [25]
  unsigned int masterCount;  //!< expected number of masters for arbitration [0]
  bool generateSyn;  //!< enable AUTO-SYN symbol generation

  const char* accessLevel;  //!< default access level
  const char* aclFile;  //!< ACL file name
  bool foreground;  //!< run in foreground
  bool enableHex;  //!< enable hex command
  bool enableDefine;  //!< enable define command
  const char* pidFile;  //!< PID file name [/var/run/ebusd.pid]
  uint16_t port;  //!< port to listen for command line connections [8888]
  bool localOnly;  //!< listen on 127.0.0.1 interface only
  uint16_t httpPort;  //!< optional port to listen for HTTP connections, 0 to disable [0]
  const char* htmlPath;  //!< path for HTML files served by the HTTP port [/var/ebusd/html]
  bool updateCheck;  //!< perform automatic update check

  const char* logFile;  //!< log file name [/var/log/ebusd.log]
  int logAreas;  //!< log areas [all]
  LogLevel logLevel;  //!< log level [notice]
  bool multiLog;  //!< multiple log levels adjusted with --log=...

  unsigned int logRaw;  //!< raw log each received/sent byte on the bus (1=messages, 2=bytes)
  const char* logRawFile;  //!< name of raw log file [/var/log/ebusd.log]
  unsigned int logRawSize;  //!< maximum size of raw log file in kB [100]

  bool dump;  //!< binary dump received bytes
  const char* dumpFile;  //!< name of dump file [/tmp/ebusd_dump.bin]
  unsigned int dumpSize;  //!< maximum size of dump file in kB [100]
  bool dumpFlush;  //!< flush each byte
};

/**
 * Get the @a DataFieldTemplates for the specified configuration file.
 * @param filename the full name of the configuration file, or "*" to get the non-root templates with the longest name
 * or the root templates if not available.
 * @return the @a DataFieldTemplates.
 */
DataFieldTemplates* getTemplates(const string& filename);

/**
 * Load the message definitions from configuration files.
 * @param messages the @a MessageMap to load the messages into.
 * @param verbose whether to verbosely log problems.
 * @param denyRecursive whether to avoid loading all files recursively (e.g. for scan config check).
 * @return the result code.
 */
result_t loadConfigFiles(MessageMap* messages, bool verbose = false, bool denyRecursive = false);

/**
 * Load the message definitions from a configuration file matching the scan result.
 * @param messages the @a MessageMap to load the messages into.
 * @param address the address of the scan participant
 * (either master for broadcast master data or slave for read slave data).
 * @param data the scan @a SlaveSymbolString for which to load the configuration file.
 * @param verbose whether to verbosely log problems.
 * @param relativeFile the string in which the name of the configuration file is stored on success.
 * @return the result code.
 */
result_t loadScanConfigFile(MessageMap* messages, symbol_t address, bool verbose, string* relativeFile);

/**
 * Helper method for executing all loaded and resolvable instructions.
 * @param messages the @a MessageMap instance.
 * @param verbose whether to verbosely log all problems.
 * @return the result code.
 */
result_t executeInstructions(MessageMap* messages, bool verbose = false);

/**
 * Helper method for loading definitions from a relative file from the config path/URL.
 * @param reader the @a FileReader instance to load with the definitions.
 * @param filename the relative name of the file being read.
 * @param verbose whether to verbosely log problems.
 * @param defaults the default values by name (potentially overwritten by file name), or nullptr to not use defaults.
 * @param errorDescription a string in which to store the error description in case of error.
 * @param replace whether to replace an already existing entry.
 * @return @a RESULT_OK on success, or an error code.
 */
result_t loadDefinitionsFromConfigPath(FileReader* reader, const string& filename, bool verbose,
    map<string, string>* defaults, string* errorDescription, bool replace = false);

}  // namespace ebusd

#endif  // EBUSD_MAIN_H_
