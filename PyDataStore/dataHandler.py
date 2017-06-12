#------------------------------------------
#--- Author: Pradeep Singh
#--- Date: 20th January 2017
#--- Version: 1.0
#--- Python Ver: 2.7
#--- Details At: https://iotbytes.wordpress.com/store-mqtt-data-from-sensors-into-sql-database/
#------------------------------------------

import MySQLdb

# MySql Sttings
DB_HOST = 'localhost' 
DB_USER = 'uniajc' 
DB_PASS = 'uniajc' 
DB_NAME = 'uniajc' 

#===============================================================
# Database Manager Class

class DatabaseManager():
	def __init__(self):
		self.conn = MySQLdb.connect(
            host=DB_HOST,    # your host, usually localhost
            user=DB_USER,    # your username
            passwd=DB_PASS,  # your password
            db=DB_NAME)
		self.cur = self.conn.cursor()
		

	def add_del_update_db_record(self, sql_query, args=()):
        # cursor.execute(query)           
		self.cur.execute(sql_query, args)   # Ejecutar una consulta
		self.conn.commit()
		return

	def __del__(self):
		self.cur.close()
		self.conn.close()

#===============================================================
# Functions to push Sensor Data into Database

def sensor_Data_Handler(Namespace, Payload):
	dbObj = DatabaseManager()
	dbObj.add_del_update_db_record("insert into arquitectura (datestamp, namespace, payload) values (NOW(),%s,%s)",(Namespace, Payload))
	del dbObj
	print "Inserted Temperature Data into Database."
	print ""

#===============================================================
