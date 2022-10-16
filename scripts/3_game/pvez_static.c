class PVEZ_Date : Managed {
	static PVEZ_Date Now() {
		int year, month, day, hour, minute;
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, NULL);
		PVEZ_Date date = new PVEZ_Date(year, month, day, hour, minute);
		return date;
	}

	int Year, Month, Day, Hour, Minute;
	void PVEZ_Date(int year = 2000, int month = 1, int day = 1, int hour = 0, int minute = 0) {
		Year = year;
		Month = month;
		Day = day;
		Hour = hour;
		Minute = minute;
	}
}

class PVEZ_StaticFunctions : Managed {

	static string GetEntityId(EntityAI entity) {
		if (!entity)
			return "";

		DayZPlayer player;
		if (Class.CastTo(player, entity)) {
			if (GetGame().IsMultiplayer()) {
				if (player.GetIdentity())
					return player.GetIdentity().GetId();
			}
			else
				return "Singleplayer";
		}

		return entity.GetID().ToString();
	}

	static string GetEntityName(EntityAI entity) {
		if (!entity)
			return "";

		DayZPlayer player;
		if (Class.CastTo(player, entity)) {
			if (GetGame().IsMultiplayer()) {
				if (player.GetIdentity())
					return player.GetIdentity().GetName();
			}
			else
				return player.GetName();
		}

		return entity.GetDisplayName();
	}

	// Returns current date+time (real world) in format: "January 1, 21:10:02"
	static string GetFormattedDateTime() {
		int year, month, day, hour, minute, second;
		GetYearMonthDay(year, month, day);
		GetHourMinuteSecond(hour, minute, second);
		string monthNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		string monthName = monthNames[month - 1];
		return monthName + " " + day + ", " + hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	}

	// Convert PVEZ_Date to string with the following format: "January 1, 21:10:02"
	static string GetFormattedDateTimeFromPVEZ_Date(PVEZ_Date dt) {
		if (!dt)
			return "";
		string monthNames[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
		string monthName = monthNames[dt.Month - 1];
		return monthName + " " + dt.Day.ToString() + ", " + dt.Hour.ToStringLen(2) + ":" + dt.Minute.ToStringLen(2);
	}

	// 1st date must be the past, 2nd date the future
	static int CountDaysBetweenDates(PVEZ_Date date1, PVEZ_Date date2) {
		int years = date1.Year - date2.Year;
		int months = date1.Month - date2.Month;
		int days = date1.Day - date2.Day;
		int result = years * 365 + months * 31 + days;
		return -result;
	}

	static int CountHoursBetweenDates(PVEZ_Date date1, PVEZ_Date date2) {
		int years = date1.Year - date2.Year;
		int months = date1.Month - date2.Month;
		int days = date1.Day - date2.Day;
		int hours = date1.Hour - date2.Hour;
		int result = years * 365 + months * 31 + days * 24 + hours;
		return -result;
	}

	static int CountMinutesBetweenDates(PVEZ_Date date1, PVEZ_Date date2) {
		int years = date1.Year - date2.Year;
		int months = date1.Month - date2.Month;
		int days = date1.Day - date2.Day;
		int hours = date1.Hour - date2.Hour;
		int minutes = date1.Minute - date2.Minute;
		int result = years * 365 + months * 31 + days * 24 + hours * 60 + minutes;
		return -result;
	}

	// Returns today's number of the day in a week.
	static int GetDayOfWeek(bool SundayFirst) {
		int c, y, m, d;
		GetYearMonthDay(y, m, d);

		c = y/100;
		if (m < 3) {
			y -= 1;
			m += 10;
		}
		else m -= 2;
		
		int dowraw = (d + Math.Floor(2.6 * m - 0.2) - 2 * c + y + Math.Floor(y / 4) + Math.Floor(c / 4));

		// Here we get the remainder of dowraw divided by 7.
		// Basically, should have been just "dowraw % 7", but DayZ says it doesn't know the % operator,
		// Yet, it is definitely used in enscript though. Odd stuff.
		int dow = Math.Floor(dowraw / 7);
		dow = dow * 7;
		dow = dowraw - dow;
		if (dow == 0) dow = 7;

		// Move days for Mon-Sun week.
		if (!SundayFirst) {
			dow -= 1;
			if (dow == 0) dow = 7;
		}

		return dow;
	}
}