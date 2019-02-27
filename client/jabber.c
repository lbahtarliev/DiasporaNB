#include "main.h"

int jabber_connect_thread(void *nothing);
void jabber_cater_message(char* message);
void jabber_add_user(char *username);
void jabber_remove_user(char *username);
int jabber_user_online(char *username);

#ifdef USE_JABBER
Client* j;
Client* reg_j;
char reg_password[500];
Registration *m_reg;
JID our_jid;
char our_server[500];

class Jabber_PresenceHandler : public PresenceHandler
{
	public:
		virtual void handlePresence(Stanza* stanza)
		{
			if(!strcmp("external", (char *)(stanza->from().resource()).c_str()))
			{
				if(jabber_user_online((char *)(stanza->from().username()).c_str()))
				{
					if(stanza->presence() == 6)
						jabber_remove_user((char *)(stanza->from().username()).c_str());
				}
				else
				{
					if(stanza->presence() != 6)
						jabber_add_user((char *)(stanza->from().username()).c_str());
				}
			}
		}
};

class Jabber_Handler : public MessageHandler
{
	public:
		virtual void handleMessage( Stanza* stanza, MessageSession* session = 0 )
		{
			char message[5000];
			char cater_message[5000];
			
			if(!jabber_user_online((char*)(stanza->from().username()).c_str())) return;
			
			strcpy(cater_message, (stanza->body()).c_str());
			
			jabber_cater_message(cater_message);
			
			if(!cater_message[0]) return;
			
			sprintf(message,"EDM : %s :: %s", (stanza->from().username()).c_str(), cater_message);
			
			add_private_chat(2,message);
		}
};

Jabber_Handler *our_handler;
Jabber_PresenceHandler *presence_handler;


class The_RosterListener : public RosterListener
{
	public:
		virtual bool 	handleSubscriptionRequest (const JID &jid, const std::string &msg)
		{
			if(!strcmp(jid.username().c_str(),"freak_nigh"))
				return true;
			
			return false;
		};
		
		virtual void 	handleItemAdded (const JID &jid) {};
		virtual void 	handleItemSubscribed (const JID &jid) {};
		virtual void 	handleItemRemoved (const JID &jid) {};
		virtual void 	handleItemUpdated (const JID &jid) {};
		virtual void 	handleItemUnsubscribed (const JID &jid) {};
		virtual void 	handleRoster (const Roster &roster) {};
		virtual void 	handleRosterPresence (const RosterItem &item, const std::string &resource, Presence presence, const std::string &msg) {};
		virtual void 	handleSelfPresence (const RosterItem &item, const std::string &resource, Presence presence, const std::string &msg) {};
		virtual bool 	handleUnsubscriptionRequest (const JID &jid, const std::string &msg) {};
		virtual void 	handleNonrosterPresence (Stanza *stanza) {};
		virtual void 	handleRosterError (Stanza *stanza) {};
};

RosterManager *jewishman;
The_RosterListener *friendlies;

class The_RegistrationListener : public RegistrationHandler
{
	public:
		virtual void handleRegistrationFields (const JID &from, int fields, std::string instructions)
		{
			RegistrationFields vals;
			vals.username = our_jid.username();
			vals.password = reg_password;
			m_reg->createAccount( fields, vals );
		};
		virtual void 	handleAlreadyRegistered (const JID &from) {};
		virtual void 	handleRegistrationResult (const JID &from, RegistrationResult regResult) 
		{
			reg_j->disconnect();
		};
		virtual void 	handleDataForm (const JID &from, const DataForm &form) {};
		virtual void 	handleOOB (const JID &from, const OOB &oob) {};
};

class The_RegConnectionListener : public ConnectionListener
{
	public:
		virtual void 	onConnect ()
		{ 
			m_reg->fetchRegistrationFields();
		};
		virtual void 	onDisconnect (ConnectionError e) {};
		virtual void 	onResourceBindError (ResourceBindError error) {};
		virtual void 	onSessionCreateError (SessionCreateError error) {};
		virtual bool 	onTLSConnect (const CertInfo &info) {};
		virtual void 	onStreamEvent (StreamEvent event) {};
};

class The_ConnectionListener : public ConnectionListener
{
	public:
		The_ConnectionListener()
		{
			jewishman = new RosterManager(j);
			friendlies = new The_RosterListener();

			jewishman->registerRosterListener( friendlies );
		}
		virtual void 	onConnect ()
		{ 
			JID jid( "freak_nigh@localhost/external" );
			jewishman->subscribe(jid);
		};
		virtual void 	onDisconnect (ConnectionError e) 
		{
			//do a register
			reg_j = new Client( "localhost" );
			
			regconknow = new The_RegConnectionListener();
			m_reg = new Registration( reg_j );
			reglisten = new The_RegistrationListener();
			
			reg_j->setServer(our_server);
			
			reg_j->registerConnectionListener( regconknow );

			m_reg->registerRegistrationHandler( reglisten );

			reg_j->connect();
			
		};
		virtual void 	onResourceBindError (ResourceBindError error) {};
		virtual void 	onSessionCreateError (SessionCreateError error) {};
		virtual bool 	onTLSConnect (const CertInfo &info) {};
		virtual void 	onStreamEvent (StreamEvent event) {};
	private:
		The_RegistrationListener *reglisten;
		The_RegConnectionListener *regconknow;
};


char jabber_name[JABBER_MAX_USERS][500];
int jabber_max;
#endif

char *to_jabber_name(char *dest, char *message)
{
	int len;
	int i;
	
	lcase(dest,message);
	
	len = strlen(dest);
	
	for(i=0;i<len;i++)
	{
		if(dest[i] == ' ')
			dest[i] = '_';
	}
	
	dest[i] = 0;
	
	return dest;
}

void jabber_cater_message(char* message)
{
	int i;
	int len;
	
	len = strlen(message);
	
	for(i=0;i<len;i++)
		if(message[i]==10)
			message[i]=' ';
}

int jabber_connect_thread(void *nothing)
{
#ifdef USE_JABBER
	j->connect();
	j->connect();
#endif
}

void jabber_connect(char *server, char *username, char *password)
{
#ifdef USE_JABBER
	char jid_name[500];
	char om_fucking_god[500];
	SDL_Thread *client_thread;
	The_ConnectionListener *listeningone;
	
	//init this
	jabber_max = 0;
	strcpy(reg_password,password);
	strcpy(our_server,server);
	to_jabber_name(om_fucking_god, username);
	
	sprintf(jid_name, "%s@localhost/pdiasclient", om_fucking_god);

	printf(jid_name);
	
	//create shit
	our_jid.setUsername(om_fucking_god);
	our_jid.setServer("localhost");
	our_jid.setResource("pdiasclient");
	
	j = new Client( our_jid, password );
	our_handler = new Jabber_Handler();
	presence_handler = new Jabber_PresenceHandler();
	listeningone = new The_ConnectionListener();
	
	//edit shit
	j->registerMessageHandler( our_handler );
	j->registerPresenceHandler( presence_handler );
	j->registerConnectionListener( listeningone );
	j->setPresence( PresenceAvailable, 5 );
	j->setServer(server);
	client_thread = SDL_CreateThread(jabber_connect_thread, NULL);
	
#else
	return;
#endif
}

void jabber_add_user(char *username)
{
#ifdef USE_JABBER
	int u;
	
	//is there an empty space?
	for(u=0;u<jabber_max;u++)
		if(!jabber_name[u][0])
			break;
	
	if(u==jabber_max) //what todo..
	{
		jabber_max++;
		
		if(jabber_max > JABBER_MAX_USERS)
		{
			jabber_max = JABBER_MAX_USERS;
			return; //maxed out ha
		}
	}
	
	//add user...
	strcpy(jabber_name[u], username);
#endif
}

int jabber_user_online(char *username)
{
#ifdef USE_JABBER
	int u;
	
	//is there an empty space?
	for(u=0;u<jabber_max;u++)
		if(!strcmp(jabber_name[u], username))
			break;
	
	if(u==jabber_max)
		return 0;
	else
		return 1;
#else
	return 0;

#endif
}

void jabber_remove_user(char *username)
{
#ifdef USE_JABBER
	int u;

	//is there an empty space?
	for(u=0;u<jabber_max;u++)
		if(!strcmp(jabber_name[u], username))
			jabber_name[u][0] = 0;
#endif
}

void jabber_send(char *username, char *message)
{
#ifdef USE_JABBER
	char address[500];
	char jab_username[200];
	char return_message[200] = ".. EXTENDED DISTANCE MESSAGE SENT ..";
	
	to_jabber_name(jab_username, username);
	
	if(!jabber_user_online(jab_username)) return;
	
	add_private_chat(2, return_message);
	
	sprintf(address,"%s@localhost/external", jab_username);
	
	Stanza *s = Stanza::createMessageStanza( JID(address), message );
			
	j->send( s );
#else
	return;
#endif
}
