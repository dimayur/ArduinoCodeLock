using Arduino_API.DBSet;
using Arduino_API.Model;
using Microsoft.AspNetCore.Mvc;
using System.Collections.Generic;
using System.Linq;

namespace MyApi.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class UserController : ControllerBase
    {
        private readonly AppDbContext _context;

        public UserController(AppDbContext context)
        {
            _context = context;
        }

        [HttpGet]
        public IEnumerable<UserModule> Get()
        {
            return _context.Users.ToList();
        }

        [HttpGet("{id}")]
        public IActionResult Get(int id)
        {
            var user = _context.Users.Find(id);

            if (user == null)
                return NotFound();

            return Ok(user);
        }

        [HttpPost]
        public IActionResult Post([FromBody] UserModule user)
        {
            _context.Users.Add(user);
            _context.SaveChanges();

            return CreatedAtAction(nameof(Get), new { id = user.Id }, user);
        }

        [HttpDelete("{id}")]
        public IActionResult Delete(int id)
        {
            var user = _context.Users.Find(id);

            if (user == null)
                return NotFound();

            _context.Users.Remove(user);
            _context.SaveChanges();

            return NoContent();
        }
    }
}
