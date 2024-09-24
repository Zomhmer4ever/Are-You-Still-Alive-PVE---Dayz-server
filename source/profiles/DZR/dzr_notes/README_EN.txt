=== How to give a note with your text to the starting gear? ===

Add this code to your init.c into the StartingEquipSetup class after SetRandomHealth( itemEnt );:

			itemEnt = player.GetInventory().CreateInInventory( "DZR_PaperWrittenAdmin_Red" );
			DZR_PaperWrittenAdmin_Red red_note = DZR_PaperWrittenAdmin_Red.Cast( itemEnt );
			WrittenNoteData m_NoteContents = new WrittenNoteData(red_note);
			m_NoteContents.SetNoteText("Here goes your text\r\nAnd the second line. Use \r\n to start new lines of thext.");
			red_note.SetPaperWrittenText(m_NoteContents);


=== How to place a permanent read-only note that is persistent forever and only admin can edit it? ===

Use DZR_3MDuctTape on your note, it will get locked forever and noone will be able to take, edit, or remove it. It will be stored persistently even after server restarts. Unlock the note using DZR_3MDuctTape again. It can be used to lock any other notes as well.

See video for demo:
https://www.youtube.com/watch?v=91lok8P8r9A


=== How to change text in admin notes offline, without connecting to the server? ===

1. Go to DZRdzr_notesstatic_notes	ype1 in your server profiles folder.
2. Create a TXT file. Rename it to your liking without spaces and special characters.
3. Edit that file and write the text you want to see in-game on your static note. No empty lines are supported! If you need an empty line, place space character on an empty line. Otherwise, the text will get cut after a new empty line.
4. In-game: Use Paper and the DZR_AdminPen_Static item to create a note. Styled admin notes are available too when using other pens: DZR_AdminPen_Red_Static, DZR_AdminPen_Green_Static, DZR_AdminPen_Blue_Static.
5. Copypaste the name of your txt file in the note text without (!) .txt extension and press OK to save it.
			For example, if you file is "Test1.txt", it should be "Test1" in the note.
6. Now, anytime a player reads the that note, the player will see not the filename, but the text from the TXT file on your server. Any changes in that TXT file on your server will be live immediately in-game.

See video for demo:
https://www.youtube.com/watch?v=a0ejbQB4JMA
https://www.youtube.com/watch?v=trN_sW4Uovg

