use C_MMORPG;

drop table if exists PlayerAccount;
drop table if exists PlayerCharacter;
drop table if exists ItemInventoryPage;
drop table if exists ItemInventory;


create table ItemInventory
(
	id int not null,
	primary key (id)
);

--Store item data in binary blob
create table ItemInventoryPage
(
	id int not null,
	page_number int not null,
	item_data blob,
	primary key (id),
	owner_id int,
	foreign key (owner_id) references ItemInventory(id)
);


--Store player character information
create table PlayerCharacter
(
	id int not null,
	credits int not null,
	primary key (id),
	inventory_id int,
	foreign key (inventory_id) references ItemInventory(id)

);

--Store player login information
create table PlayerAccount
(
	id int not null,
	username varchar(50) not null,
	password_hash binary(32),
	password_salt varchar(32),
	user_role int not null,
	primary key (id),
	character_id int,
	foreign key (character_id) references PlayerCharacter(id)
);



