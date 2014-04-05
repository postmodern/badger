def usage
  puts "usage: badger [OPTIONS]"
end

while (arg = ARGV.shift)
  case arg
  when '-V', '--version'
    puts "badger #{Badger::VERSION}"
    exit
  when '-h', '--help'
    usage
    exit
  when nil
    $stderr.puts "badger: no options specified"
    exit -1
  else
    $stderr.puts "badger: unknown option: #{arg}"
    exit -1
  end
end
