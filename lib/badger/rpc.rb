module Badger
  module RPC
    def self.serialize(data);   JSON.generate(data);  end
    def self.deserialize(data); JSON.parse(data); end
    def self.decode_request(request)
      request = deserialize(request)

      return request['name'], request.key?('arguments') ? request['arguments'] : []
    end

    def self.Serve(line)
      name, arguments = decode_request(line)
      #puts "name:#{name} , args:#{arguments}"

      send_response(line,call(name,arguments))
    end

    def self.send_response(socket,response)
      message = serialize(response) + "\0"
    end

    def self.call(name,arguments)
    #unless (method = self[name])
     # return {'exception' => "Unknown method: #{name}"}
    #end

    names    = name.split('.')
    mod = names[0]
    function = names[1]
    print "#{mod} #{function} #{arguments}\n"

    value = begin
      case mod

        when 'Shell' then
          RPC::Shell.send("#{function}", *arguments)

        when 'Fs' then
          RPC::Fs.send("#{function}", *arguments)

        when 'Proc' then
          RPC::Proc.send("#{function}", *arguments)

        when 'Net' then
          RPC::Net.send("#{function}", *arguments)

        when 'Ruby' then
          RPC::Ruby.send("#{function}", *arguments)


        else return {'exception' => "Module #{mod} not found"}
      end
      rescue Exception => exception
      return {'exception' => exception.message}

    end
    return {'return' => value}
    end
  end
end


