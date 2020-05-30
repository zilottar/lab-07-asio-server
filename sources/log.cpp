//Copyright 2020 <LinkIvan333>
#include "include/log.h"

void init()
{
  boost::log::core::get()->add_global_attribute(
      "TimeStamp", boost::log::attributes::local_clock());
  auto sinkFile = boost::log::add_file_log(
      keywords::file_name = "logs/log_info.log",
      keywords::rotation_size = 128 * 1024 * 1024,
      keywords::format =
          (boost::log::expressions::stream
           << boost::log::expressions::format_date_time<
                  boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
           << ": <" << boost::log::trivial::severity << "> "
           << boost::log::expressions::smessage));
  sinkFile->set_filter(logging::trivial::severity >= logging::trivial::info);

  auto sinkConsole = logging::add_console_log(
      std::cout,
      keywords::format =
          (expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                               "TimeStamp", "%Y-%m-%d %H:%M:%S")
                        << ": <" << logging::trivial::severity << "> "
                        << expr::smessage));
  sinkConsole->set_filter(logging::trivial::severity >= logging::trivial::info);
  logging::add_common_attributes();
}

void logInfoClientsMessage(const std::string& data) {
    BOOST_LOG_TRIVIAL(info) << data << "\n";
}
