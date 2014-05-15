 module Bagder
  module RPC
    module Proc
      def self.getpid;             ::Bager::Process.pid;         end
      def self.getppid;            ::Process.ppid;        end
      def self.getuid;             ::Process.uid;         end
      def self.setuid(uid);        ::Process.uid = uid;   end
      def self.geteuid;            ::Process.euid;        end
      def self.seteuid(euid);      ::Process.euid = euid; end
      def self.getgid;             ::Process.gid;         end
      def self.setgid(gid);        ::Process.gid = gid;   end
      def self.getegid;            ::Process.egid;        end
      def self.setegid(gid);       ::Process.egid = egid; end
      def self.getsid;             ::Process.sid;         end
      def self.setsid(sid);        ::Process.sid = sid;   end
      def self.getenv(name);       ENV[name];             end
      def self.setenv(name,value); ENV[name] = value;     end
      def self.unsetenv(name);     ENV.delete(name);      end

      def self.kill(pid,signal='KILL'); ::Process.kill(pid,signal); end
      def self.getcwd;                  Dir.pwd;                    end
      def self.chdir(path);             Dir.chdir(path);            end
      def self.time;                    Time.now.to_i;              end
      def self.spawn(program,*arguments)
        fork { exec(program,*arguments) }
      end
      def self.exit; exit; end
    end
  end
end
