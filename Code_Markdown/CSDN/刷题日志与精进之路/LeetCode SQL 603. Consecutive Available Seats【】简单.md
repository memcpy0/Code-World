Several friends at a cinema ticket office would like to reserve consecutive available seats.<br>
Can you help to query all the consecutive available seats order by the seat_id using the following <code>cinema</code> table?

```haskell
| seat_id | free |
|---------|------|
| 1       | 1    |
| 2       | 0    |
| 3       | 1    |
| 4       | 1    |
| 5       | 1    | 
```

Your query should return the following result for the sample case above.
 

```haskell
| seat_id |
|---------|
| 3       |
| 4       |
| 5       |
```

<b>Note</b>:

<ul>
	<li>The seat_id is an auto increment int, and free is bool ('1' means free, and '0' means occupied.).</li>
	<li>Consecutive available seats are more than 2(inclusive) seats consecutively available.</li>
</ul>


