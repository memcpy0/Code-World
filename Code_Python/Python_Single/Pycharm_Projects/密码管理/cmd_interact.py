
fieldnames = ('name', 'account_name', 'password')
maxfield = max(len(f) for f in fieldnames)

db = shelve.open("password-classes-shelve")

while True:
    key = input("\nKey => ")
    if not key : break
    if key in db:
        record = db[key]
        if 'q' in input("To query(q) or to change(c)? => [q or c]"):
            for field in fieldnames:
               print(field.ljust(maxfield), "=>", getattr(record, field))
            print(record)
            break

    else:
        record = PasswordManager(name='?', account_name='?', password='?')

    for field in fieldnames:
        currval = getattr(record, field)
        newtext = input('\t[%s]=%s\n\tnew?=>' % (field, currval))
        if newtext:
            setattr(record, field, eval(newtext))
    db[key] = record

db.close()

