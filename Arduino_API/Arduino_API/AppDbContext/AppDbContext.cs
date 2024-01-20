using Arduino_API.Model;
using Microsoft.EntityFrameworkCore;
using System.Collections.Generic;

namespace Arduino_API.DBSet
{
    public class AppDbContext : DbContext
    {
        public AppDbContext(DbContextOptions<AppDbContext> options)
           : base(options)
        {

        }
        public virtual DbSet<UserModule> Users { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
          
        }
    }
}
 