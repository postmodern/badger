module Badger
		module RPC
		      

		   def self.serialize(data);   JSON.generate(data);     end
		   def self.deserialize(data); JSON.parse(data); end

		   def self.decode_request(request)
		      	request = deserialize(request)

		      	return request['name'], request.key?('arguments') ? request['arguments'] : []


		   end

		 



      def self.Serve(line)
          name, arguments = decode_request(line)
          puts "name:#{name} , args:#{arguments}"

          send_response(line,call(name,arguments))
        
      end

        def self.send_response(socket,response)
        message = serialize(response) + "\0"
        #socket.write(message)



       end
        

    
    end


    def RPC.call(name,arguments)
    #unless (method = self[name])
     # return {'exception' => "Unknown method: #{name}"}
    #end

    names    = name.split('.')
    mod = names[0]
    function = names[1]
    #puts "yoyo"
    #puts mod


    value = begin
               case mod
               when 'Net' then
               RPC::Net.send("#{function}", *arguments)

              

               when 'Shell' then
               RPC::Shell.send("#{function}", *arguments)
     

               else return {'exception' => "Module #{mod} not found"}
               end
               rescue => exception
               return {'exception' => exception.message}
          
            end

      
  
    return {'return' => value}
  end



  
end


