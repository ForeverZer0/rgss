
module RGSS

  require "logger"

  ##
  # The logger instance for RGSS.
  Log = Logger.new(STDOUT)

  Log.formatter = proc do |severity, datetime, progname, msg|
    time = datetime.strftime("%H:%M:%S")
    # Add a little color to the output with some basic ANSI escape sequences.
    prefix = case severity
      when "DEBUG" then "[\33[1;35mDEBUG\33[0m]"
      when "INFO" then "[\33[1;32mINFO\33[0m] "
      when "WARN" then "[\33[1;33mWARN\33[0m] "
      when "ERROR" then "[\33[1;31mERROR\33[0m]"
      when "FATAL" then "[\33[1;31mFATAL\33[0m]"
      else "[\33[1;32mINFO\33[0m] "
      end
    "#{time} #{prefix} #{msg}\n"
  end

  # TODO; Read from environment variable and/or commandline args of executable
  Log.level = $RGSS_DEBUG ? Logger::DEBUG : Logger::INFO
end